#include "heat_solver.hpp"

namespace heat_equation
{

HeatSolver::HeatSolver(int width, int height) : width_(width), height_(height), diffusionConstant_(0.75f)
{
  temperature_.resize(height_, std::vector<float>(width_, 0.0f));
  temperatureNext_.resize(height_, std::vector<float>(width_, 0.0f));
}

HeatSolver::~HeatSolver() {}

void HeatSolver::update(float dt)
{
  for (int y = 1; y < height_ - 1; ++y)
  {
    for (int x = 1; x < width_ - 1; ++x)
    {
        float left = temperature_.at(y).at(x - 1);
        float right = temperature_.at(y).at(x + 1);
        float top = temperature_.at(y - 1).at(x);
        float bottom = temperature_.at(y + 1).at(x);
        float center = temperature_.at(y).at(x);

        float laplacian = left + right + top + bottom - 4.0f * center;

        temperatureNext_.at(y).at(x) = center + diffusionConstant_ * laplacian * dt;
    }
  }
  std::swap(temperature_, temperatureNext_);
}

void HeatSolver::setHeatSource(int x, int y, float temperature)
{
  if (x >= 0 && x < width_ && y >= 0 && y < height_)
  {
    temperature_.at(y).at(x) = temperature;
  }
}

const std::vector<std::vector<float>>& HeatSolver::getTemperatureGrid() const 
{
  return temperature_;
}

} // namespace heat_equation