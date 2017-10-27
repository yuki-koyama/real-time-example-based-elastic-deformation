// Original source code by Kenshi Takayama

// special case of 1x1 matrix (considered as convertible to scalar) ===========================-
template <typename Scalar>
class Matrix<Scalar, 1, 1> {
public:
    typedef Scalar value_type;
    typedef Matrix<Scalar, 1, 1> matrix_type;
private:
    value_type value_;
public:
    // constructors ===============
    Matrix() { fill(0); }
    // copy constructor and substitution operator are default
    explicit Matrix(const value_type& s): value_(s) {}
    ~Matrix() {}
    
    // setter =================
    void set(const value_type& s) { value_ = s; }
    
    // fill ====================
    void fill(const value_type& s) { value_ = s; }
    
    // cast to raw pointer ====================
    value_type* ptr() { return &value_; }
    const value_type* ptr() const { return &value_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { assert(index == 0); return value_; }
    const value_type& operator[](size_t index) const { assert(index == 0); return value_; }
    value_type& operator()(size_t row, size_t col) { assert(row == 0 && col == 0); return value_; }
    const value_type& operator()(size_t row, size_t col) const { assert(row == 0 && col == 0); return value_; }
    
    // arithmetic (member) =====================
    // with scalar
    matrix_type& operator+=(const value_type& rhs) { value_ += rhs; return *this; }
    matrix_type& operator-=(const value_type& rhs) { value_ -= rhs; return *this; }
    matrix_type& operator*=(const value_type& rhs) { value_ *= rhs; return *this; }
    matrix_type& operator/=(const value_type& rhs) { value_ /= rhs; return *this; }
    matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
    matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
    matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
    // negate
    matrix_type operator-() const { return matrix_type(-value_); }
    
    // matrix operations ==================
    // transpose
    matrix_type transpose() const { return *this; }
    value_type determinant() const { return value_; }
    matrix_type invert() const {
        if (value_ == 0.0) throw "Matrix is singular! ";
        return matrix_type(1/value_);
    }
};
