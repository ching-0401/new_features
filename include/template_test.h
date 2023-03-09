/*************************************
 * author:      yuki
 * data:        2022.11.08
 * email:       jujuyu0401@outlook.com
 ************************************/


#ifndef YUKI_NEW_FEATURES_TEMPLATE_TEST__H
#define YUKI_NEW_FEATURES_TEMPLATE_TEST__H

#include <iostream>
#include <type_traits>

namespace yuki_new_features__template_test
{
    // partial template specialization, template specialization
    template<typename T, typename P>
    class Test
    {
    public:
        void show(T a_t, P a_p)
        {
            std::cout << "typename T, typename P" << std::endl;
        }
    };

    template <typename T>

    class Test<T, std::string>
    {
    public:
        void show(T a_t, std::string a_p)
        {
            std::cout << "typename T, std::string P" << std::endl;
        }
    };

    template<>
    class Test<std::string, std::string>
    {
    public:
        void show(std::string a_t, std::string a_p)
        {
            std::cout << "std::string T, std::string P" << std::endl;
        }
    };


    // type traits
    struct PODStruct
    {
        bool isPOD(){return true;}
    };

    struct NoPODStruct
    {
        bool isPOD(){return false;}
    };

    template<typename T>
    class TraitsTest
    {
    public:
        typedef T Data;
        void show(Data a_t)
        {
            std::cout << "POD? " << a_t.isPOD() << std::endl;
        }
    };



    auto f = []()->int{
        std::cout << "111" << std::endl;
        return 1;
    };

//    auto ty = std::result_of<f()>::type;


}




















#endif
