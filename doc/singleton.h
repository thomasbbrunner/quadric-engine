class S
{
  public:
    static S &get_instance()
    {
        static S instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
        return instance;
    }

  private:
    S() {} // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
  public:
    S(S const &) = delete;
    void operator=(S const &) = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
};