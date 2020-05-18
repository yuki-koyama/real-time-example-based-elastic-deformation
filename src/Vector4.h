// Original source code by Kenshi Takayama

// 4D vector (x, y, z, w) ===========================
template <typename Scalar> class Vector<Scalar, 4>
{
public:
    typedef Scalar            value_type;
    typedef Vector<Scalar, 4> vector_type;
    value_type                x_, y_, z_, w_;

    static const size_t size_ = 4; // OpenMesh compatible

    // constructors ===============
    Vector() { fill(0); }
    Vector(const vector_type& src) { operator=(src); }
    // template<typename OtherScalar>
    // explicit Vector(const Vector<OtherScalar, N>& src) { operator=(src); }        // this constructor may lead to
    // bugs...
    explicit Vector(value_type s) { fill(s); }
    explicit Vector(const value_type v[4])
    {
        x_ = v[0];
        y_ = v[1];
        z_ = v[2];
        w_ = v[3];
    }
    template <int NHigh> explicit Vector(const Vector<Scalar, NHigh>& vecHigh)
    {
        assert(4 < NHigh);
        x_ = vecHigh[0];
        y_ = vecHigh[1];
        z_ = vecHigh[2];
        w_ = vecHigh[3];
    }
    ~Vector() {}

    // dimension-wise constructors =================
    Vector(value_type x, value_type y, value_type z, value_type w) { set(x, y, z, w); }
    Vector(const Vector<Scalar, 2>& v_head, const Vector<Scalar, 2>& v_tail) { set(v_head, v_tail); }
    Vector(Scalar head, const Vector<Scalar, 3>& v_tail) { set(head, v_tail); }
    Vector(const Vector<Scalar, 3>& v_head, Scalar tail) { set(v_head, tail); }

    // dimension-wise setters =================
    void set(value_type x, value_type y, value_type z, value_type w)
    {
        x_ = x;
        y_ = y;
        z_ = z;
        w_ = w;
    }
    void set(const Vector<Scalar, 2>& v_head, const Vector<Scalar, 2>& v_tail)
    {
        x_ = v_head.x_;
        y_ = v_head.y_;
        z_ = v_tail.x_;
        w_ = v_tail.y_;
    }
    void set(Scalar head, const Vector<Scalar, 3>& v_tail)
    {
        x_ = head;
        y_ = v_tail.x_;
        z_ = v_tail.y_;
        w_ = v_tail.z_;
    }
    void set(const Vector<Scalar, 3>& v_head, Scalar tail)
    {
        x_ = v_head.x_;
        y_ = v_head.y_;
        z_ = v_head.z_;
        w_ = tail;
    }

    // substitution operators ===================
    vector_type& operator=(const vector_type& src)
    {
        if (this != &src)
        {
            x_ = src.x_;
            y_ = src.y_;
            z_ = src.z_;
            w_ = src.w_;
        }
        return *this;
    }

    // type conversion ==========================
    template <typename OtherScalar> Vector<OtherScalar, 4> convert() const
    {
        Vector<OtherScalar, 4> result;
        result.x_ = static_cast<OtherScalar>(x_);
        result.y_ = static_cast<OtherScalar>(y_);
        result.z_ = static_cast<OtherScalar>(z_);
        result.w_ = static_cast<OtherScalar>(w_);
        return result;
    }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const
    {
        vector_type d = (*this) - rhs;
        if (d.x_ <= -epsilon || epsilon <= d.x_ || d.y_ <= -epsilon || epsilon <= d.y_ || d.z_ <= -epsilon ||
            epsilon <= d.z_ || d.w_ <= -epsilon || epsilon <= d.w_)
            return false;
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
        if (z_ < rhs.z_)
            return true;
        else if (rhs.z_ < z_)
            return false;
        if (w_ < rhs.w_) return true;
        return false;
    }
    bool operator<=(const vector_type& rhs) const { return *this == rhs || *this < rhs; }
    bool operator>(const vector_type& rhs) const { return rhs < *this; }
    bool operator>=(const vector_type& rhs) const { return rhs <= *this; }

    // fill ====================
    void fill(const value_type& s) { x_ = y_ = z_ = w_ = s; }
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
        w_ += rhs;
        return *this;
    }
    vector_type& operator-=(value_type rhs)
    {
        x_ -= rhs;
        y_ -= rhs;
        z_ -= rhs;
        w_ -= rhs;
        return *this;
    }
    vector_type& operator*=(value_type rhs)
    {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }
    vector_type& operator/=(value_type rhs)
    {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        w_ /= rhs;
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
        w_ += rhs.w_;
        return *this;
    }
    vector_type& operator-=(const vector_type& rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        w_ -= rhs.w_;
        return *this;
    }
    vector_type& operator*=(const vector_type& rhs)
    {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        z_ *= rhs.z_;
        w_ *= rhs.w_;
        return *this;
    }
    vector_type& operator/=(const vector_type& rhs)
    {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        z_ /= rhs.z_;
        w_ /= rhs.w_;
        return *this;
    }
    vector_type operator+(const vector_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const vector_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const vector_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const vector_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const { return vector_type(-x_, -y_, -z_, -w_); }

    // vector operation =========================
    // dot product
    value_type operator|(const vector_type& rhs) const { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_ + w_ * rhs.w_; }
    value_type dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    // normalize
    value_type  norm() const { return static_cast<value_type>(sqrt(sqnorm())); }
    value_type  sqnorm() const { return (*this) | (*this); }
    value_type  length() const { return norm(); }
    value_type  lengthSquared() const { return sqnorm(); }
    vector_type normalized() const { return (*this) / norm(); }
    void        normalize() { (*this) /= norm(); }
    void        normalize_cond()
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
        result        = w_ < result ? w_ : result;
        return result;
    }
    value_type elemMax() const
    {
        Scalar result = -(std::numeric_limits<Scalar>::min());
        result        = result < x_ ? x_ : result;
        result        = result < y_ ? y_ : result;
        result        = result < z_ ? z_ : result;
        result        = result < w_ ? w_ : result;
        return result;
    }
    value_type  elemSum() const { return x_ + y_ + z_ + w_; }
    value_type  elemMean() const { return elemSum() / 4; }
    vector_type pairwiseMax(const vector_type& rhs) const
    {
        vector_type result;
        result.x_ = x_ < rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ < rhs.y_ ? rhs.y_ : y_;
        result.z_ = z_ < rhs.z_ ? rhs.z_ : z_;
        result.w_ = w_ < rhs.w_ ? rhs.w_ : w_;
        return result;
    }
    vector_type pairwiseMin(const vector_type& rhs) const
    {
        vector_type result;
        result.x_ = x_ > rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ > rhs.y_ ? rhs.y_ : y_;
        result.z_ = z_ > rhs.z_ ? rhs.z_ : z_;
        result.w_ = w_ > rhs.w_ ? rhs.w_ : w_;
        return result;
    }
    void set_random(value_type range_min, value_type range_max)
    {
        value_type range_size = range_max - range_min;
        x_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        y_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        z_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        w_                    = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
    }
    Vector<Scalar, 2> xy() const { return Vector<Scalar, 2>(x_, y_); }
    Vector<Scalar, 3> xyz() const { return Vector<Scalar, 3>(x_, y_, z_); }
    // append ====================
    template <int M> Vector<Scalar, 4 + M> append(const Vector<Scalar, M>& tail) const
    {
        Vector<Scalar, 4 + M> result;
        result[0] = x_;
        result[1] = y_;
        result[2] = z_;
        result[3] = w_;
        for (size_t i = 0; i < M; ++i)
            result[4 + i] = tail[i];
        return result;
    }
};
