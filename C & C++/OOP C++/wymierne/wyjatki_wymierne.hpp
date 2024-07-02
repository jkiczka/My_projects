
class wyjatek_wymierny : public std::logic_error
{
    const char* str;
public:
    explicit wyjatek_wymierny(const char* what) : logic_error(what) {};
    virtual const char* what() const noexcept   { return str; };
};

class dzielenie_przez_zero : public wyjatek_wymierny
{
public:
    explicit dzielenie_przez_zero(const char* what) : wyjatek_wymierny(what) {};
    const char* what() const noexcept   { return "Dzielenie przez 0"; };
};

class zakres : public wyjatek_wymierny
{
public:
    explicit zakres (const char* what) : wyjatek_wymierny(what) {};
    const char* what() const noexcept   { return "Wykonane operacje powoduja wyjscie poza zakres"; };
};
