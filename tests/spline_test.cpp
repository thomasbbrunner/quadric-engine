#include <stdio.h>
#include <Eigen/Core>
#include <Eigen/Splines>
#include <iostream>

double uvalue(double x, double low, double high)
{
    return (x - low) / (high - low);
}

Eigen::VectorXd uvalues(Eigen::VectorXd xvals)
{
    const double low = xvals.minCoeff();
    const double high = xvals.maxCoeff();
    for (int i = 0; i < xvals.size(); ++i)
    {
        xvals(i) = uvalue(xvals(i), low, high);
    }
    return xvals;
}

int main()
{
    Eigen::VectorXd x_pts(9);
    Eigen::VectorXd y_pts(9);
    Eigen::VectorXd z_pts(9);

    x_pts << 0.0, 1.77, 6.6, 7.5, 8.78, 10, 12, 16, 20;
    y_pts << 1, 1.3, 20.2, 15, 15, 22, 5, -3, -20;
    z_pts << 0, 2.2, 4.2, 1.5, -0.3, -0.4, -1.7, -3, -5;

    Eigen::MatrixXd pts(3, 9);

    pts.row(0) = x_pts;
    pts.row(1) = y_pts;
    pts.row(2) = z_pts;

    std::cout << pts << std::endl;

    Eigen::Spline<double, 3> spline;
    spline = Eigen::SplineFitting<Eigen::Spline<double, 3>>::Interpolate(pts, 3);

    double step = 0.01;
    double x_max = 1 + step;

    for (double x = 0; x < x_max; x += step)
    {
        Eigen::VectorXd y_data = spline(x);

        std::cout << y_data[0] << " " << y_data[1] << " " << y_data[2] << ";\n";
    }

    return 0;
}