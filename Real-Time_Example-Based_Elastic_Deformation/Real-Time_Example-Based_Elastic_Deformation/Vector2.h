// Original source code by Kenshi Takayama

// 2D vector (x, y) ===========================
template <typename Scalar>
class Vector<Scalar, 2> {
public:
    typedef Scalar value_type;
    typedef Vector<Scalar, 2> vector_type;
    value_type x_, y_;
    
    static const size_t size_ = 2;  // OpenMesh compatible
    
    // constructors ===============
    Vector() { fill(0); }
    Vector(const vector_type& src) { operator=(src); }
    //template<typename OtherScalar>
    //explicit Vector(const Vector<OtherScalar, N>& src) { operator=(src); }        // this constructor may lead to bugs...
    explicit Vector(value_type s) { fill(s); }
    explicit Vector(const value_type v[2]) { x_ = v[0]; y_ = v[1]; }
    template <int NHigh>
    explicit Vector(const Vector<Scalar, NHigh>& vecHigh) {
        assert(2 < NHigh);
        x_ = vecHigh[0];
        y_ = vecHigh[1];
    }
    ~Vector() {}
    
    // dimension-wise constructors =================
    Vector(value_type x, value_type y) { set(x, y); }

    // dimension-wise setters =================
    void set(value_type x, value_type y) {
        x_ = x;
        y_ = y;
    }
    
    // substitution operators ===================
    vector_type& operator=(const vector_type& src) {
        if (this != &src) {
            x_ = src.x_;
            y_ = src.y_;
        }
        return *this;
    }
    
    // type conversion ==========================
    template<typename OtherScalar>
    Vector<OtherScalar, 2> convert() const {
        Vector<OtherScalar, 2> result;
        result.x_ = static_cast<OtherScalar>(x_);
        result.y_ = static_cast<OtherScalar>(y_);
        return result;
    }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const {
        Scalar dx = x_ - rhs.x_;
        if (dx <= -epsilon || epsilon <= dx)
            return false;
        Scalar dy = y_ - rhs.y_;
        if (dy <= -epsilon || epsilon <= dy)
            return false;
        return true;
    }
    // dictionary-order comparators
    bool operator<(const vector_type& rhs) const {
        if (x_ < rhs.x_) return true; else if (rhs.x_ < x_) return false;
        if (y_ < rhs.y_) return true;
        return false;
    }
    bool operator<=(const vector_type& rhs) const { return *this == rhs || *this < rhs; }
    bool operator>(const vector_type& rhs) const { return rhs < *this; }
    bool operator>=(const vector_type& rhs) const { return rhs <= *this; }
    
    // fill ====================
    void fill(const value_type& s) { x_ = y_ = s; }
    void vectorize(const value_type& s) { fill(s); }    // for OpenMesh compatibility
    
    // cast to raw pointer ====================
    value_type* ptr() { return &x_; }
    const value_type* ptr() const { return &x_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return (&x_)[index]; }
    const value_type& operator[](size_t index) const { return (&x_)[index]; }
    
    // arithmetic (member) =====================
    // with scalar
    vector_type& operator+=(value_type rhs) { x_ += rhs; y_ += rhs; return *this; }
    vector_type& operator-=(value_type rhs) { x_ -= rhs; y_ -= rhs; return *this; }
    vector_type& operator*=(value_type rhs) { x_ *= rhs; y_ *= rhs; return *this; }
    vector_type& operator/=(value_type rhs) { x_ /= rhs; y_ /= rhs; return *this; }
    vector_type operator+(value_type rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(value_type rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(value_type rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(value_type rhs) const { return vector_type(*this) /= rhs; }
    // with vector
    vector_type& operator+=(const vector_type& rhs) { x_ += rhs.x_; y_ += rhs.y_; return *this; }
    vector_type& operator-=(const vector_type& rhs) { x_ -= rhs.x_; y_ -= rhs.y_; return *this; }
    vector_type& operator*=(const vector_type& rhs) { x_ *= rhs.x_; y_ *= rhs.y_; return *this; }
    vector_type& operator/=(const vector_type& rhs) { x_ /= rhs.x_; y_ /= rhs.y_; return *this; }
    vector_type operator+(const vector_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const vector_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const vector_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const vector_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const {
        return vector_type(-x_, -y_);
    }
    
    // vector operation =========================
    // dot product
    value_type operator|(const vector_type& rhs) const {
        return
            x_ * rhs.x_ +
            y_ * rhs.y_;
    }
    value_type dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    // normalize
    value_type norm() const { return static_cast<value_type>(sqrt(sqnorm())); }
    value_type sqnorm() const { return (*this) | (*this); }
    value_type length() const { return norm(); }
    value_type lengthSquared() const { return sqnorm(); }
    vector_type normalized() const { return (*this) / norm(); }
    void normalize()  { (*this) /= norm(); }
    void normalize_cond() {
        value_type r = norm();
        (*this) /= (r == static_cast<value_type>(0)) ? 1 : r;
    }

    // max / min / sum / mean ==========================
    value_type elemMin() const {
        Scalar result = (std::numeric_limits<Scalar>::max());
        result = x_ < result ? x_ : result;
        result = y_ < result ? y_ : result;
        return result;
    }
    value_type elemMax() const {
        Scalar result = -(std::numeric_limits<Scalar>::min());
        result = result < x_ ? x_ : result;
        result = result < y_ ? y_ : result;
        return result;
    }
    value_type elemSum() const {
        return x_ + y_;
    }
    value_type elemMean() const { return elemSum() / 2; }
    vector_type pairwiseMax(const vector_type& rhs) const {
        vector_type result;
        result.x_ = x_ < rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ < rhs.y_ ? rhs.y_ : y_;
        return result;
    }
    vector_type pairwiseMin(const vector_type& rhs) const {
        vector_type result;
        result.x_ = x_ > rhs.x_ ? rhs.x_ : x_;
        result.y_ = y_ > rhs.y_ ? rhs.y_ : y_;
        return result;
    }
    void set_random(value_type range_min, value_type range_max) {
        value_type range_size = range_max - range_min;
        x_ = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
        y_ = range_min + static_cast<value_type>(rand() * range_size / RAND_MAX);
    }
    // append ====================
    template <int M>
    Vector<Scalar, 2 + M> append(const Vector<Scalar, M>& tail) const {
        Vector<Scalar, 2 + M> result;
        result[0] = x_;
        result[1] = y_;
        for (size_t i = 0; i < M; ++i)
            result[2 + i] = tail[i];
        return result;
    }
};
