#ifndef FLAPPY_BLOCK_TYPEPROXY_HPP
#define FLAPPY_BLOCK_TYPEPROXY_HPP

template<typename T>
class TypeProxy {
private:
    T value;
    bool changeFlag;
public:
    TypeProxy(const T &val)
        :value(val)
    {

    }
    TypeProxy(T &&val = T())
        :value(val)
    {

    }

    TypeProxy(TypeProxy<T> &&rhs) {
        this->value = rhs.value;
    }

    TypeProxy &operator=(const T &val) {
        changeFlag = true;
        value = val;
        return *this;
    }
    TypeProxy &operator=(T &&val) {
        changeFlag = true;
        value = val;
        return *this;
    }

    operator const T&() const noexcept {
        return value;
    }

    operator T&() noexcept {
        return value;
    }

    bool changed() const noexcept {
        return changeFlag;
    }

    void resetChangeFlag() noexcept {
        changeFlag = false;
    }

    bool getChangeFlagAndReset() noexcept {
        auto f = changeFlag;
        resetChangeFlag();
        return  f;
    }
};


#endif //FLAPPY_BLOCK_TYPEPROXY_HPP
