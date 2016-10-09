class safeSingleton
{
  static std::shared_ptr& safeSingleton & instance_;
  static std::once_flag only_one;
 
  safeSingleton(int id) {
    std::cout << "safeSingleton::Singleton()" << id << std::endl;
  }
 
  safeSingleton(const safeSingleton& rs) {
    instance_ = rs.instance_;
  }
 
  safeSingleton& operator = (const safeSingleton& rs)
  {
    if (this != &rs) {
        instance_ = rs.instance_;
    }
    
    return *this;
  }
 
  public:
    ~safeSingleton() {
    std::cout << "Singleton::~Singleton" << std::endl;
    }
 
  static safeSingleton & getInstance( int id )
  {
    std::call_once( safeSingleton::only_one,
                    [] (int idx)
                    {
                        safeSingleton::instance_.reset( new safeSingleton(idx) );
 
                        std::cout << "safeSingleton::create_singleton_() | thread id " + idx << std::endl;
                    }, id );
 
    return *safeSingleton::instance_;
  }
 
  void demo(int id) { std::cout << "demo stuff from thread id " << id << std::endl; }
};
 
std::once_flag safeSingleton::only_one;
std::shared_ptr& safeSingleton& safeSingleton::instance_ = nullptr;