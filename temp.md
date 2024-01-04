#### Lennard-Jones potential

$$
u(r) = 4\epsilon\left(\left(\frac{\sigma}{r}\right)^{12}-\left(\frac{\sigma}{r}\right)^{6}\right)\\
\mathbf{F}(\mathbf{r}) = 4\epsilon\left(12\frac{\sigma^{12}}{r^{13}} - 6\frac{\sigma^6}{r^7}\right)\hat{\mathbf{r}}
$$

#### Kinetic temperature

$$
kT(t)=\frac{2}{d}\frac{K(t)}{N}=\frac{1}{Nd}\sum_{i=1}^Nm_i\mathbf{v}_i(t)\cdot\mathbf{v}_i(t)
$$

for 2D
$$
kT=\frac{1}{2N}\sum m_i\overline{\mathbf{v}_i\cdot \mathbf{v}_i}
$$

$$
k = 1.380649\times 10^{-23} J/K
$$

#### Proj

A viscous damping force
$$
f_{ij}=-\gamma(\mathbf{v}_{ij}\cdot\mathbf{r}_{ij})\frac{\mathbf{r}_{ij}}{r_{ij}^2}
$$

$$
\gamma = 100\\
m_i = 1
$$

kinetic temperature
$$
T = \frac{1}{N}\sum m_i \overline{\mathbf{v}_i\cdot \mathbf{v}_i}
$$

$$
N = 64, L = 20, \Delta t = 0.001, T_0 = 10
$$

