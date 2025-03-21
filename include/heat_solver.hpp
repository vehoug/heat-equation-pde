#ifndef HEAT_SOLVER_HPP
#define HEAT_SOLVER_HPP

#include <algorithm>
#include <vector>

namespace heat_equation
{

class HeatSolver
{
  public:
    HeatSolver(int width, int height);
    ~HeatSolver();

    void update(float dt);
    void setHeatSource(int x, int y, float temperature);

    const std::vector<std::vector<float>>& getTemperatureGrid() const;
    int getWidth() const;
    int getHeight() const;

  private:
    int width_;
    int height_;

    std::vector<std::vector<float>> temperature_;
    std::vector<std::vector<float>> temperatureNext_;

    const float diffusionConstant_;

};

} // namespace heat_equation

#endif // HEAT_SOLVER_HPP