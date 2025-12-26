#include <open3d/Open3D.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include "sim/particles.h"
namespace o3d = open3d;

double GetRandom() { return double(std::rand()) / double(RAND_MAX); }

std::shared_ptr<o3d::geometry::PointCloud> MakePointCloud(
    int npts, const Eigen::Vector3d center, double radius, bool colorize) {
  auto cloud = std::make_shared<o3d::geometry::PointCloud>();
  cloud->points_.reserve(npts);
  for (int i = 0; i < npts; ++i) {
    cloud->points_.push_back({radius * GetRandom() + center.x(),
                              radius * GetRandom() + center.y(),
                              radius * GetRandom() + center.z()});
  }
  if (colorize) {
    cloud->colors_.reserve(npts);
    for (int i = 0; i < npts; ++i) {
      cloud->colors_.push_back({GetRandom(), GetRandom(), GetRandom()});
    }
  }
  return cloud;
}

int main() {
  o3d::visualization::VisualizerWithKeyCallback visualizer;
  visualizer.CreateVisualizerWindow("Particle System", 1600, 900, 50, 50);

  const size_t num_particles{1000};
  // const size_t num_steps{5000};
  const double d_t{1.};
  Particles<> par_sys(num_particles, d_t);

  std::vector<double> Fext_x(num_particles);
  std::vector<double> Fext_y(num_particles);
  std::vector<double> Fext_z(num_particles);
#ifdef DEBUG
  std::fill_n(std::begin(Fext_x), num_particles, 0.);
  std::fill_n(std::begin(Fext_y), num_particles, 0.);
  std::fill_n(std::begin(Fext_z), num_particles, 0.);
#else
  RNG<double> rng;
  rng.GenerateUniformRandom(Fext_x.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_y.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_z.data(), num_particles, 0., 0.);
#endif
  // auto start_the_clock = std::chrono::system_clock::now();

  auto point_cloud = std::make_shared<o3d::geometry::PointCloud>();
  point_cloud->points_.resize(num_particles);
  // Add the initial points
  for (size_t i = 0; i < num_particles; ++i) {
    point_cloud->points_[i] = {par_sys.x[i], par_sys.y[i], par_sys.z[i]};
  }
  visualizer.AddGeometry(point_cloud);

  auto &option = visualizer.GetRenderOption();
  option.point_size_ = 5.0;

  std::atomic<bool> running{true};
  // ESC : close window
  visualizer.RegisterKeyCallback(GLFW_KEY_ESCAPE, [&](auto *) {
    running = false;
    return false;  // keep default ESC behavior
  });

  // SPACE : pause / resume
  std::atomic<bool> paused{false};
  visualizer.RegisterKeyCallback(GLFW_KEY_SPACE, [&](auto *) {
    paused = !paused;
    return false;
  });
  // b : change background color
  std::atomic<bool> black_bg{false};
  visualizer.RegisterKeyCallback(GLFW_KEY_B, [&](auto *) {
    if (!black_bg) {
      option.background_color_ = Eigen::Vector3d(0, 0, 0);
      black_bg = true;
    } else {
      option.background_color_ = Eigen::Vector3d(1, 1, 1);
      black_bg = false;
    }
    return false;
  });

  bool first = true;
  while (running && visualizer.PollEvents()) {
    if (!paused) {
      par_sys.Update(Fext_x, Fext_y, Fext_z, 0., 0, 0.);
      // TODO Parallel copy
      for (size_t i = 0; i < num_particles; ++i) {
        point_cloud->points_[i] = {par_sys.x[i], par_sys.y[i], par_sys.z[i]};
      }
      visualizer.UpdateGeometry(point_cloud);

      // do once after first non-empty geometry
      if (first) {
        visualizer.ResetViewPoint(true);
        first = false;
      }
    }
    visualizer.UpdateRender();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  visualizer.DestroyVisualizerWindow();
  return 0;
}
