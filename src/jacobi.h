#pragma once

// Originally, this code is from http://www.alecrivers.com/fastlsm/ .

#include <cmath>

namespace Y
{

#define TOL 1e-3f
#define MAX_SWEEPS 3

    inline void jacobi(unsigned int n, float* a, float* d, float* v)
    {
        float        onorm, dnorm;
        float        b, dma, q, t, c, s;
        float        atemp, vtemp, dtemp;
        register int i, j, k, l;

        // Set v to the identity matrix, set the vector d to contain the
        // diagonal elements of the matrix a
        d[0] = a[0];
        d[1] = a[4];
        d[2] = a[8];

        for (l = 1; l <= MAX_SWEEPS; l++)
        {
            // Set dnorm to be the maximum norm of the diagonal elements, set
            // onorm to the maximum norm of the off-diagonal elements

            dnorm = (float) fabs(d[0]) + (float) fabs(d[1]) + (float) fabs(d[2]);
            onorm = (float) fabs(a[1]) + (float) fabs(a[2]) + (float) fabs(a[5]);
            // Normal end point of this algorithm.
            if ((onorm / dnorm) <= TOL)
            {
                return;
            }

            for (j = 1; j < (int) n; j++)
            {
                for (i = 0; i <= j - 1; i++)
                {
                    b = a[n * i + j];
                    if (fabs(b) > 0.0f)
                    {
                        dma = d[j] - d[i];
                        if ((fabs(dma) + fabs(b)) <= fabs(dma))
                        {
                            t = b / dma;
                        }
                        else
                        {
                            q = 0.5f * dma / b;
                            t = 1.0f / ((float) fabs(q) + (float) sqrt(1.0f + q * q));
                            if (q < 0.0) t = -t;
                        }

                        c            = 1.0f / (float) sqrt(t * t + 1.0f);
                        s            = t * c;
                        a[n * i + j] = 0.0f;

                        for (k = 0; k <= i - 1; k++)
                        {
                            atemp        = c * a[n * k + i] - s * a[n * k + j];
                            a[n * k + j] = s * a[n * k + i] + c * a[n * k + j];
                            a[n * k + i] = atemp;
                        }

                        for (k = i + 1; k <= j - 1; k++)
                        {
                            atemp        = c * a[n * i + k] - s * a[n * k + j];
                            a[n * k + j] = s * a[n * i + k] + c * a[n * k + j];
                            a[n * i + k] = atemp;
                        }

                        for (k = j + 1; k < (int) n; k++)
                        {
                            atemp        = c * a[n * i + k] - s * a[n * j + k];
                            a[n * j + k] = s * a[n * i + k] + c * a[n * j + k];
                            a[n * i + k] = atemp;
                        }

                        for (k = 0; k < (int) n; k++)
                        {
                            vtemp        = c * v[n * k + i] - s * v[n * k + j];
                            v[n * k + j] = s * v[n * k + i] + c * v[n * k + j];
                            v[n * k + i] = vtemp;
                        }

                        dtemp = c * c * d[i] + s * s * d[j] - 2.0f * c * s * b;
                        d[j]  = s * s * d[i] + c * c * d[j] + 2.0f * c * s * b;
                        d[i]  = dtemp;
                    }
                }
            }
        }
        return;
    }

#undef TOL
#undef MAX_SWEEPS

} // namespace Y
