// Original source code by Kenshi Takayama

// 3D vector (x, y, z) ===========================
template <typename Scalar> class Vector<Scalar, 3>
{
public:
    typedef Scalar            value_type;
    typedef Vector<Scalar, 3> vector_type;
    union {
        struct
        {
            value_type x_, y_, z_;
        };
        struct
        {
            value_type x, y, z;
        };
        value_type val[3];
    };

    static const size_t size_ = 3; // OpenMesh compatible

    // constructors ===============
    Vector() { fill(0); }
    Vector(const vector_type& src) { operator=(src); }
    // template<typename OtherScalar>
    // explicit Vector(const Vector<OtherScalar, N>& src) { operator=(src); }        // this constructor may lead to
    // bugs...
    explicit Vector(value_type s) { fill(s); }
    explicit Vector(const value_type v[3])
    {
        x_ = v[0];
        y_ = v[1];
        z_ = v[2];
    }
    explicit Vector(const Vector<Scalar, 2>& vec)
    {
        x_ = vec[0];
        y_ = vec[1];
        z_ = 0;
    }
    template <int NHigh> explicit Vector(const Vector<Scalar, NHigh>& vecHigh)
    {
        assert(3 < NHigh);
        x_ = vecHigh[0];
        y_ = vecHigh[1];
        z_ = vecHigh[2];
    }
    ~Vector() {}

    // dimension-wise constructors =================
    Vector(value_type x, value_type y, value_type z) { set(x, y, z); }

    // dimension-wise setters =================
    void set(value_type x, value_type y, value_type z)
    {
        x_ = x;
        y_ = y;
        z_ = z;
    }
    void set(Scalar head, const Vector<Scalar, 2>& v_tail)
    {
        x_ = head;
        y_ = v_tail.x_;
        z_ = v_tail.y_;
    }
    void set(const Vector<Scalar, 2>& v_head, Scalar tail)
    {
        x_ = v_head.x_;
        y_ = v_head.y_;
        z_ = tail;
    }

    // substitution operators ===================
    vector_type& operator=(const vector_type& src)
    {
        if (this != &src)
        {
            x_ = src.x_;
            y_ = src.y_;
            z_ = src.z_;
        }
        return *this;
    }

    // type conversion ==========================
    template <typename OtherScalar> Vector<OtherScalar, 3> convert() const
    {
        Vector<OtherScalar, 3> result;
        result.x_ = static_cast<OtherScalar>(x_);
        result.y_ = static_cast<OtherScalar>(y_);
        result.z_ = static_cast<OtherScalar>(z_);
        return result;
    }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const
    {
        Scalar dx = x_ - rhs.x_;
        if (dx <= -epsilon || epsilon <= dx) return false;
        Scalar dy = y_ - rhs.y_;
        if (dy <= -epsilon || epsilon <= dy) return false;
        Scalar dz = z_ - rhs.z_;
        if (dz <= -epsilon || epsilon <= dz) return false;
        return true;
    }
    // dictionary-order comparators
    bool operator<(const vector_type& rhs) const
    {
        if (x_ < rhs.x_)
            return true;
        else if (rhs.x_ < x_)
            return false;
        if (y_ < rhs.y_)
            return true;
        else if (rhs.y_ < y_)
            return false;
        if (z_ < rhs.z_) return true;
        return false;
    }
    bool operator<=(const vector_type& rhs) const { return *this == rhs || *this < rhs; }
    bool operator>(const vector_type& rhs) const { return rhs < *this; }
    bool operator>=(const vector_type& rhs) const { return rhs <= *this; }

    // fill ====================
    void fill(const value_type& s) { x_ = y_ = z_ = s; }
    void vectorize(const value_type& s) { fill(s); } // for OpenMesh compatibility

    // cast to raw pointer ====================
    value_type*       ptr() { return &x_; }
    const value_type* ptr() const { return &x_; }

    // indexing ==========================
    value_type&       operator[](size_t index) { return (&x_)[index]; }
    const value_type& operator[](size_t index) const { return (&x_)[index]; }

    // arithmetic (member) =====================
    // with scalar
    vector_type& operator+=(value_type rhs)
    {
        x_ += rhs;
        y_ += rhs;
        z_ += rhs;
        return *this;
    }
    vector_type& operator-=(value_type rhs)
    {
        x_ -= rhs;
        y_ -= rhs;
        z_ -= rhs;
        return *this;
    }
    vector_type& operator*=(value_type rhs)
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        return *this;
    }
    vector_type& operator/=(value_type rhs)
    {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        return *this;
    }
    vector_type operator+(value_type rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(value_type rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(value_type rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(value_type rhs) const { return vector_type(*this) /= rhs; }
    // with vector
    vector_type& operator+=(const vector_type& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }
    vector_type& operator-=(const vector_type& rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }
    vector_type& operator*=(const vector_type& rhs)
    {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        z_ *= rhs.z_;
        return *this;
    }
    vector_type& operator/=(const vector_type& rhs)
    {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        z_ /= rhs.z_;
        return *this;
    }
    vector_type operator+(const vector_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const vector_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const vector_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const vector_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const { return vector_type(-x_, -y_, -z_); }

    // vector operation =========================
    // dot product
    value_type        operator|(const vector_type& rhs) const { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_; }
    value_type        dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    friend value_type dot(const vector_type& rhs1, const vector_type& rhs2)
    {
        return (rhs1) | (rhs2);
    } // for OpenMesh compatibility
    vector_type cross_product(const vector_type& rhs) const { return (*this) % (rhs); }
    // normalize
    value_type  norm() const { return static_cast<value_type>(sqrt(sqnorm())); }
    value_type  sqnorm() const { return (*this) | (*this); }
    value_type  length() const { return norm(); }
    value_type  lengthSquared() const { return sqnorm(); }
    vector_type normalized() const { return (*this) / norm(); }
    //    void normalize()  { (*this) /= norm(); }                  // for OpenMesh compatibility!
    vector_type normalize()
    {
        (*this) /= norm();
        return *this;
    } // for OpenMesh compatibility!
    void normalize_cond()
    {
        value_type r = norm();
        (*this) /= (r == static_cast<value_type>(0)) ? 1 : r;
    }

    // max / min / sum / mean ==========================
    value_type elemMin() const
    {
        Scalar result = (std::numeric_limits<Scalar>::max());
        result        = x_ < result ? x_ : result;
        result        = y_ < result ? y_ : result;
        result        = z_ < result ? z_ : result;
        return result;
    }
    value_type elemMax() const
    {
        Scalar result = -(std::numeric_limits<Scalar>::min());
        result        = result < x_ ? x_ : result;
        result        = result < y_ ? y_ : result;
        result        = result < z_ ? z_ : result;
        return result;
    }
    value_type  elemSum() const { return x_ + y_ + z_; }
    value_type  elemMean() const { return elemSum() / 3; }
    vector_type pairwiseMax(const vector_type& rhs) const
    {
        vector_type result;
        result.x_ = x_ < rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ < rhs.y_ ? rhs.y_ : y_;
        result.z_ = z_ < rhs.z_ ? rhs.z_ : z_;
        return result;
    }
    vector_type pairwiseMin(const vector_type& rhs) const
    {
        vector_type result;
        result.x_ = x_ > rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ > rhs.y_ ? rhs.y_ : y_;
        result.z_ = z_ > rhs.z_ ? rhs.z_ : z_;
        return result;
    }
    void set_random(value_type range_min, value_type range_max)
    {
        value_type range_size = range_max - range_min;
        x_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        y_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        z_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
    }
    Vector<Scalar, 2> xy() const { return Vector<Scalar, 2>(x_, y_); }
    // append ====================
    template <int M> Vector<Scalar, 3 + M> append(const Vector<Scalar, M>& tail) const
    {
        Vector<Scalar, 3 + M> result;
        result[0] = x_;
        result[1] = y_;
        result[2] = z_;
        for (size_t i = 0; i < M; ++i)
            result[3 + i] = tail[i];
        return result;
    }
};
