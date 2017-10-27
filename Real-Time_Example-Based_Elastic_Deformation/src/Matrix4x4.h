// Original source code by Kenshi Takayama

template <typename Scalar>
class Matrix<Scalar, 4, 4> {
public:
    typedef Scalar value_type;
    typedef Matrix<Scalar, 4, 4> matrix_type;

    // modified by Yuki Koyama //
    union {
        value_type values_[16];
        value_type m[4][4];
    };
    /////////////////////////////
    
    // constructors ===============
    Matrix() { fill(0); }
    Matrix(const matrix_type& src) { operator=(src); }
    template<typename OtherScalar>
    explicit Matrix(const Matrix<OtherScalar, 4, 4>& src) { operator=(src); }
    explicit Matrix(const value_type& s) { fill(s); }
    explicit Matrix(const value_type v[16]) { memcpy(values_, v, 16 * sizeof(value_type)); }
    explicit Matrix(const Vector<Scalar, 4>& diagonal) { setDiagonal(diagonal); }
    explicit Matrix(const Vector<Scalar, 4>& vecRow, const Vector<Scalar, 4>& vecCol) { setTensorProd(vecRow, vecCol); }
    ~Matrix() {}
    
    // dimension-wise constructors =================
    Matrix(
        const value_type& s00, const value_type& s01, const value_type& s02, const value_type& s03,
        const value_type& s10, const value_type& s11, const value_type& s12, const value_type& s13,
        const value_type& s20, const value_type& s21, const value_type& s22, const value_type& s23,
        const value_type& s30, const value_type& s31, const value_type& s32, const value_type& s33)
    {
        set(
            s00, s01, s02, s03,
            s10, s11, s12, s13,
            s20, s21, s22, s23,
            s30, s31, s32, s33);
    }

    // dimension-wise setters =================
    void set(
        const value_type& s00, const value_type& s01, const value_type& s02, const value_type& s03,
        const value_type& s10, const value_type& s11, const value_type& s12, const value_type& s13,
        const value_type& s20, const value_type& s21, const value_type& s22, const value_type& s23,
        const value_type& s30, const value_type& s31, const value_type& s32, const value_type& s33)
    {
        values_[ 0] = s00;    values_[ 1] = s01;    values_[ 2] = s02;    values_[ 3] = s03;
        values_[ 4] = s10;    values_[ 5] = s11;    values_[ 6] = s12;    values_[ 7] = s13;
        values_[ 8] = s20;    values_[ 9] = s21;    values_[10] = s22;    values_[11] = s23;
        values_[12] = s30;    values_[13] = s31;    values_[14] = s32;    values_[15] = s33;
    }
    void setTensorProd(const Vector<Scalar, 4>& vecRow, const Vector<Scalar, 4>& vecCol) {
        for (size_t i = 0; i < 4; ++i) for (size_t j = 0; j < 4; ++j) values_[4 * i + j] = vecRow[j] * vecCol[i];
    }
    void setDiagonal(const Vector<Scalar, 4>& diagonal) {
        set(
            diagonal[0], 0, 0, 0,
            0, diagonal[1], 0, 0,
            0, 0, diagonal[2], 0,
            0, 0, 0, diagonal[3]);
    }
    void setIdentity() {
        set(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }
    void setRow(size_t rowIndex, const Vector<Scalar, 4>& rowValue) {
        memcpy(&values_[4 * rowIndex], rowValue.ptr(), 4 * sizeof(value_type));
    }
    void setCol(size_t colIndex, const Vector<Scalar, 4>& colValue) {
        for (size_t row = 0; row < 4; ++row) (*this)(row, colIndex) = colValue[row];
    }
    Vector<Scalar, 4> getRow(size_t rowIndex) const {
        Vector<Scalar, 4> result;
        memcpy(result.ptr(), &values_[4 * rowIndex], 4 * sizeof(value_type));
        return result;
    }
    Vector<Scalar, 4> getCol(size_t colIndex) const {
        Vector<Scalar, 4> result;
        for (size_t row = 0; row < 4; ++row) result[row] = (*this)(row, colIndex);
        return result;
    }
    void setRows(
        const Vector<Scalar, 4>& rowValue0,
        const Vector<Scalar, 4>& rowValue1,
        const Vector<Scalar, 4>& rowValue2,
        const Vector<Scalar, 4>& rowValue3)
    {
        set(
            rowValue0[0], rowValue0[1], rowValue0[2], rowValue0[3],
            rowValue1[0], rowValue1[1], rowValue1[2], rowValue1[3],
            rowValue2[0], rowValue2[1], rowValue2[2], rowValue2[3],
            rowValue3[0], rowValue3[1], rowValue3[2], rowValue3[3]);
    }
    void setCols(
        const Vector<Scalar, 4>& colValue0,
        const Vector<Scalar, 4>& colValue1,
        const Vector<Scalar, 4>& colValue2,
        const Vector<Scalar, 4>& colValue3)
    {
        set(
            colValue0[0], colValue1[0], colValue2[0], colValue3[0],
            colValue0[1], colValue1[1], colValue2[1], colValue3[1],
            colValue0[2], colValue1[2], colValue2[2], colValue3[2],
            colValue0[3], colValue1[3], colValue2[3], colValue3[3]);
    }
    
    // substitution operators ===================
    matrix_type& operator=(const matrix_type& src) {
        if (this != &src) memcpy(values_, src.values_, 16 * sizeof(value_type));
        return *this;
    }
    template<typename OtherScalar>
    matrix_type& operator=(const Matrix<OtherScalar, 4, 4>& src) {
        for (size_t i = 0; i < 16; ++i) values_[i] = static_cast<value_type>(src.values_[i]);
        return *this;
    }
    
    // fill ====================
    void fill(const value_type& s) { std::fill(values_, values_ + 16, s); }
    
    // cast to raw pointer ====================
    value_type* ptr() { return values_; }
    const value_type* ptr() const { return values_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    value_type& operator()(size_t row, size_t col) { return values_[4 * row + col]; }
    const value_type& operator()(size_t row, size_t col) const { return values_[4 * row + col]; }
    
    // comparison ======================
    bool operator==(const matrix_type& rhs) const {
        for (size_t i = 0; i < 16; ++i) if (values_[i] != rhs.values_[i]) return false;
        return true;
    }
    bool operator!=(const matrix_type& rhs) const { return !operator==(rhs); }

    // arithmetic (member) =====================
    // with scalar
    matrix_type& operator+=(const value_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] += rhs; return *this; }
    matrix_type& operator-=(const value_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] -= rhs; return *this; }
    matrix_type& operator*=(const value_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] *= rhs; return *this; }
    matrix_type& operator/=(const value_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] /= rhs; return *this; }
    matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
    matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
    matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
    // with same sized matrix
    matrix_type& operator+=(const matrix_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] += rhs.values_[i]; return *this; }
    matrix_type& operator-=(const matrix_type& rhs) { for (size_t i = 0; i < 16; ++i) values_[i] -= rhs.values_[i]; return *this; }
    matrix_type operator+(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const matrix_type& rhs) const { return matrix_type(*this) -= rhs; }
    // mult with vector
    Vector<Scalar, 4> operator*(const Vector<Scalar, 4>& rhs) const {
        Vector<Scalar, 4> result;
        for (size_t row = 0; row < 4; ++row) for (size_t col = 0; col < 4; ++col) result[row] += (*this)(row, col) * rhs[col];
        return result;
    }
    Vector<Scalar, 3> transform(const Vector<Scalar, 3>& rhs) const {
        Vector<Scalar, 3> result;
        for (size_t row = 0; row < 3; ++row) {
            for (size_t col = 0; col < 3; ++col)
                result[row] += (*this)(row, col) * rhs[col];
            result[row] += (*this)(row, 3);
        }
        return result;
    }
    // mult with matrix
    template<int NColNew>
    Matrix<Scalar, 4, NColNew> operator*(const Matrix<Scalar, 4, NColNew>& rhs) { 
        Matrix<Scalar, 4, NColNew> result;
        for (size_t row = 0; row < 4; ++row) for (size_t col = 0; col < NColNew; ++col) {
            Scalar sum = 0;
            for (size_t k = 0; k < 4; ++k) sum += (*this)(row, k) * rhs(k, col);
            result(row, col) = sum;
        }
        return result;
    }
    // negate
    matrix_type operator-() const {
        matrix_type result;
        for (size_t i = 0; i < 16; ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // matrix operations ==================
    // transpose
    matrix_type transpose() const {
        matrix_type result;
        for (size_t row = 0; row < 4; ++row) for (size_t col = 0; col < 4; ++col) result(col, row) = operator()(row, col);
        return result;
    }
    value_type determinant() const {
        const matrix_type& m = *this;
        return
             (m(0,0)*m(1,1) - m(0,1)*m(1,0)) * (m(2,2)*m(3,3) - m(2,3)*m(3,2))
            -(m(0,0)*m(1,2) - m(0,2)*m(1,0)) * (m(2,1)*m(3,3) - m(2,3)*m(3,1))
            +(m(0,0)*m(1,3) - m(0,3)*m(1,0)) * (m(2,1)*m(3,2) - m(2,2)*m(3,1))
            +(m(0,1)*m(1,2) - m(0,2)*m(1,1)) * (m(2,0)*m(3,3) - m(2,3)*m(3,0))
            -(m(0,1)*m(1,3) - m(0,3)*m(1,1)) * (m(2,0)*m(3,2) - m(2,2)*m(3,0))
            +(m(0,2)*m(1,3) - m(0,3)*m(1,2)) * (m(2,0)*m(3,1) - m(2,1)*m(3,0));
    }
    bool invert() {
        Scalar s = determinant();
        if (s == 0.0) return false;
        s = 1/s;
        matrix_type m(*this);
        this->set(
            m(1,1)*(m(2,2)*m(3,3) - m(2,3)*m(3,2)) + m(1,2)*(m(2,3)*m(3,1) - m(2,1)*m(3,3)) + m(1,3)*(m(2,1)*m(3,2) - m(2,2)*m(3,1)),
            m(2,1)*(m(0,2)*m(3,3) - m(0,3)*m(3,2)) + m(2,2)*(m(0,3)*m(3,1) - m(0,1)*m(3,3)) + m(2,3)*(m(0,1)*m(3,2) - m(0,2)*m(3,1)),
            m(3,1)*(m(0,2)*m(1,3) - m(0,3)*m(1,2)) + m(3,2)*(m(0,3)*m(1,1) - m(0,1)*m(1,3)) + m(3,3)*(m(0,1)*m(1,2) - m(0,2)*m(1,1)),
            m(0,1)*(m(1,3)*m(2,2) - m(1,2)*m(2,3)) + m(0,2)*(m(1,1)*m(2,3) - m(1,3)*m(2,1)) + m(0,3)*(m(1,2)*m(2,1) - m(1,1)*m(2,2)),

            m(1,2)*(m(2,0)*m(3,3) - m(2,3)*m(3,0)) + m(1,3)*(m(2,2)*m(3,0) - m(2,0)*m(3,2)) + m(1,0)*(m(2,3)*m(3,2) - m(2,2)*m(3,3)),
            m(2,2)*(m(0,0)*m(3,3) - m(0,3)*m(3,0)) + m(2,3)*(m(0,2)*m(3,0) - m(0,0)*m(3,2)) + m(2,0)*(m(0,3)*m(3,2) - m(0,2)*m(3,3)),
            m(3,2)*(m(0,0)*m(1,3) - m(0,3)*m(1,0)) + m(3,3)*(m(0,2)*m(1,0) - m(0,0)*m(1,2)) + m(3,0)*(m(0,3)*m(1,2) - m(0,2)*m(1,3)),
            m(0,2)*(m(1,3)*m(2,0) - m(1,0)*m(2,3)) + m(0,3)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) + m(0,0)*(m(1,2)*m(2,3) - m(1,3)*m(2,2)),

            m(1,3)*(m(2,0)*m(3,1) - m(2,1)*m(3,0)) + m(1,0)*(m(2,1)*m(3,3) - m(2,3)*m(3,1)) + m(1,1)*(m(2,3)*m(3,0) - m(2,0)*m(3,3)),
            m(2,3)*(m(0,0)*m(3,1) - m(0,1)*m(3,0)) + m(2,0)*(m(0,1)*m(3,3) - m(0,3)*m(3,1)) + m(2,1)*(m(0,3)*m(3,0) - m(0,0)*m(3,3)),
            m(3,3)*(m(0,0)*m(1,1) - m(0,1)*m(1,0)) + m(3,0)*(m(0,1)*m(1,3) - m(0,3)*m(1,1)) + m(3,1)*(m(0,3)*m(1,0) - m(0,0)*m(1,3)),
            m(0,3)*(m(1,1)*m(2,0) - m(1,0)*m(2,1)) + m(0,0)*(m(1,3)*m(2,1) - m(1,1)*m(2,3)) + m(0,1)*(m(1,0)*m(2,3) - m(1,3)*m(2,0)),

            m(1,0)*(m(2,2)*m(3,1) - m(2,1)*m(3,2)) + m(1,1)*(m(2,0)*m(3,2) - m(2,2)*m(3,0)) + m(1,2)*(m(2,1)*m(3,0) - m(2,0)*m(3,1)),
            m(2,0)*(m(0,2)*m(3,1) - m(0,1)*m(3,2)) + m(2,1)*(m(0,0)*m(3,2) - m(0,2)*m(3,0)) + m(2,2)*(m(0,1)*m(3,0) - m(0,0)*m(3,1)),
            m(3,0)*(m(0,2)*m(1,1) - m(0,1)*m(1,2)) + m(3,1)*(m(0,0)*m(1,2) - m(0,2)*m(1,0)) + m(3,2)*(m(0,1)*m(1,0) - m(0,0)*m(1,1)),
            m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) + m(0,1)*(m(1,2)*m(2,0) - m(1,0)*m(2,2)) + m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0)));
        (*this) *= s;
        return true;
    }
    bool solve(const Vector<Scalar, 4>& b, Vector<Scalar, 4>& x) const {
        matrix_type inv(*this);
        if (!inv.invert()) return false;
        x = inv * b;
        return true;
    }
    // static maker ==========================
    static matrix_type identity() {
        matrix_type result;
        result.setIdentity();
        return result;
    }
    static matrix_type rotationFromAxisAngle(const Vector<Scalar, 3>& axis, Scalar angle) {
        matrix_type result;
        Vector<Scalar, 3> a(axis);
		a.normalize();
		Scalar c = cos(angle);
		Scalar s = sin(angle);
		Scalar omc = 1.0 - c;
		result(0, 0) = c + a[0] * a[0] * omc;
		result(1, 1) = c + a[1] * a[1] * omc;
		result(2, 2) = c + a[2] * a[2] * omc;
		
		Scalar tmp1 = a[0] * a[1] * omc;
		Scalar tmp2 = a[2] * s;
		result(0, 1) = tmp1 - tmp2;
		result(1, 0) = tmp1 + tmp2;
		
		tmp1 = a[0] * a[2] * omc;
		tmp2 = a[1] * s;
		result(0, 2) = tmp1 + tmp2;
		result(2, 0) = tmp1 - tmp2;
		
		tmp1 = a[1] * a[2] * omc;
		tmp2 = a[0] * s;
		result(1, 2) = tmp1 - tmp2;
		result(2, 1) = tmp1 + tmp2;
		
		result(3, 3) = 1;

        return result;
    }
    
    // added by Yuki Koyama //
    static matrix_type scaleMatrix(value_type x, value_type y, value_type z) {
        matrix_type result = identity();
        result.m[0][0] = x;
        result.m[1][1] = y;
        result.m[2][2] = z;
        return result;
    }
    static matrix_type translationMatrix(value_type x, value_type y, value_type z) {
        matrix_type result = identity();
        result.m[0][3] = x;
        result.m[1][3] = y;
        result.m[2][3] = z;
        return result;
    }
    //////////////////////////
};
