// Original source code by Kenshi Takayama

#pragma once
#include "Vector.h"
#include "Util.h"
#include <cmath>

namespace Y {
    
    class Trackball {
    public:
        enum DragMode {
            DRAGMODE_ROTATE,
            DRAGMODE_PAN,
            DRAGMODE_ZOOM,
            DRAGMODE_NONE
        };
        DragMode dragMode_;
        Vector2i prevpos_;
        int width_;
        int height_;
        Vector3d eyePoint;
        Vector3d focusPoint;
        Vector3d upDirection;
        
        Trackball()
        : width_(0)
        , height_(0)
        , dragMode_(DRAGMODE_NONE)
        , eyePoint(0, 0, 1.5)
        , focusPoint(0, 0, 0)
        , upDirection(0, 1, 0)
        {}
        Vector3d eyeDirection() const { return focusPoint - eyePoint; }
        void setSize(int width, int height) {
            width_  = width;
            height_ = height;
        }
        void mouseDown(int x, int y, DragMode dragMode = DRAGMODE_ROTATE) {
            prevpos_.set(x, y);
            dragMode_ = dragMode;
        }
        void mouseMove(int x, int y, bool updateUpDirection = false) {
            if (dragMode_ == DRAGMODE_NONE)
                return;
            Vector2i pos(x, y);
            static const double PI = 2.0 * std::asin(1.0);
            switch (dragMode_) {
                case DRAGMODE_ROTATE:
                {
                    double thetaX = (2 * PI * (pos.x_ - prevpos_.x_)) / width_;
                    double thetaY = (2 * PI * (pos.y_ - prevpos_.y_)) / height_;
                    // Horizontal rotation
                    Matrix3x3d rotH = Util::rotationFromAxisAngle(upDirection, -thetaX);
                    Vector3d eye = eyePoint - focusPoint;
                    eye = rotH * eye;
                    // Vertical rotation
                    Vector3d leftDirection = eye % upDirection;
                    Matrix3x3d rotV = Util::rotationFromAxisAngle(leftDirection, thetaY);
                    float tmp = leftDirection.dot_product((rotV * eye) % upDirection);
                    if (tmp > 0.0) {
                        eye = rotV * eye;                        
                    }

                    if (updateUpDirection) {
                        upDirection = rotV * upDirection;
                    }

                    eyePoint = focusPoint + eye;
                }
                    break;
                case DRAGMODE_PAN:
                {
                    Vector3d eye = eyePoint - focusPoint;
                    Vector3d leftDirection = eye % upDirection;
                    double len = eye.length();
                    Vector3d transX(leftDirection);
                    Vector3d transY(upDirection);
                    transX.normalize();
                    transY.normalize();
                    transX *= len * (pos.x_ - prevpos_.x_) / width_;
                    transY *= len * (pos.y_ - prevpos_.y_) / height_;
                    eyePoint += transX;
                    eyePoint += transY;
                    focusPoint += transX;
                    focusPoint += transY;
                }
                    break;
                case DRAGMODE_ZOOM:
                {
                    Vector3d eyeDirection = focusPoint - eyePoint;
                    eyeDirection *= (pos.y_ - prevpos_.y_) / static_cast<double>(height_);
                    eyePoint += eyeDirection;
                }
                    break;
                case DRAGMODE_NONE:
                {;}
            }
            prevpos_ = pos;
        }
        void mouseUp() {
            dragMode_ = DRAGMODE_NONE;
        }
    };
    
}

