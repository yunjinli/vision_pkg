# Vision Package

## Introduction

In order to get familiar with some vision-related algorithms such as different kinds of ways to compute the optical flow, I built this repository to build them from ground up based on OpenCV.

## Optical Flow

### Lukas-Kanal method

By the **Brightness Constancy Assumtion**, the intensity of the image can be represented by $\text{I}(x,y,t)$. Let say we have two consecutive frames, the intensity of the same point in the frame would be the same. $\text{I}(x,y,t) = \text{I}(x+\delta x,y+\delta y,t+\delta t)$.

From Taylor Expansion, we can rewrite $\text{I}(x+\delta x,y+\delta y,t+\delta t)$ as:

$\text{I}(x,y,t)+\frac{\partial }{\partial x}\text{I}(x,y,t)\delta x+\frac{\partial }{\partial y}\text{I}(x,y,t)\delta y +\frac{\partial }{\partial t}\text{I}(x,y,t)\delta t$

Therefore, the equation at the begining could be rewrited as $\frac{\partial }{\partial x}\text{I}(x,y,t)\delta x+\frac{\partial }{\partial y}\text{I}(x,y,t)\delta y +\frac{\partial }{\partial t}\text{I}(x,y,t)\delta t=0$.

To make the representation simpler, let $\text{I}_x=\frac{\partial }{\partial x}\text{I}(x,y,t), \text{I}_y=\frac{\partial }{\partial y}\text{I}(x,y,t), \text{I}_t=\frac{\partial }{\partial t}\text{I}(x,y,t)$

We later have $\text{I}_x \delta x + \text{I}_y \delta y + \text{I}_t \delta t=0$

We divide the equation above with $\delta t$.

we have $\text{I}_x \frac{\delta x}{\delta t} + \text{I}_y \frac{\delta y}{\delta t} + \text{I}_t=0$

However, we cannot solve $\frac{\delta x}{\delta t}$ and $\frac{\delta y}{\delta t}$ by only one equation. Hence, we introduce the second assumpution, **Constant motion in a neighborhood**

Accoding to this assumption, we can assume that the neighboring pixel of (x,y) would move at the same speed. So, we can write the new equation as follow (Now we change to vector form):

$\nabla\text{I}(\textbf{x}',t)^T\textbf{v}+\text{I}_t(\textbf{x}',t)=0 \quad \forall \textbf{x}'\in \text{Neighbor}(\textbf{x})$

By taking the integral of the entire neighboring area, we can have:

$\textbf{E}(\textbf{v})=\int _{\text{Neighbor}(\textbf{x})}||\nabla\text{I}(\textbf{x}',t)^T\textbf{v}+\text{I}_t(\textbf{x}',t)||^2d\textbf{x}'$

$\frac{d\textbf{E}}{d\textbf{v}}=2\textbf{M}\textbf{v}+2\textbf{q}=0$

$\textbf{M}=\int \nabla\text{I}(\textbf{x}',t)\nabla\text{I}(\textbf{x}',t)^Td\textbf{x}'$

$\textbf{q}=\int \text{I}_t(\textbf{x}',t)\circ\nabla\text{I}(\textbf{x}',t)d\textbf{x}'$

Solve the linear equation by inverse matrix:

$\textbf{v}=\textbf{M}^{-1}\textbf{q}$

```
for (const auto& m : matches_) {
    cv::Point p = keypoints1_[m.queryIdx].pt;
    int x_start = std::max(0, p.x - (int)(l / 2));
    int x_end = std::min(length_, p.x + (int)(l / 2));

    int y_start = std::max(0, p.y - (int)(h / 2));
    int y_end = std::min(length_, p.y + (int)(h / 2));

    double M_data[2][2] = {0, 0, 0, 0};
    double q_data[2][1] = {0, 0};
    cv::Mat M(2, 2, CV_64F, M_data);
    cv::Mat q(2, 1, CV_64F, q_data);

    // Iterate through the window
    for (int i = x_start + delta; i <= x_end - delta; i++) {
      for (int j = y_start + delta; j <= y_end - delta; j++) {
        int x1 = i - delta,
            x2 = i + delta;  // we can then calculate dx = x2 - x1
        int y1 = j - delta,
            y2 = j + delta;  // we can then calculate dy = y2 - y1

        double Ix =
            ((int)img1.at<uchar>(j, x1) - (int)img1.at<uchar>(j, x2)) / 2;
        double Iy =
            ((int)img1.at<uchar>(y1, i) - (int)img1.at<uchar>(y2, i)) / 2;
        double It = ((int)img1.at<uchar>(j, i) - (int)img2.at<uchar>(j, i));

        M.at<double>(0, 0) += Ix * Ix;
        M.at<double>(1, 1) += Iy * Iy;
        M.at<double>(0, 1) += Ix * Iy;
        M.at<double>(1, 0) += Iy * Ix;
        q.at<double>(0, 0) += It * Ix;
        q.at<double>(1, 0) += It * Iy;
      }
    }
    double v_data[2][1] = {0, 0};
    cv::Mat v(2, 1, CV_64F, v_data);
    v = -M.inv() * q;
```
