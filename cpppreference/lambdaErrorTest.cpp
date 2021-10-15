//
// Created by william on 2021/4/17.
//

#include "base.h"

using namespace std;

#define STR(T) #T
#define DEBUG_PRINT_THIS(T) \
T() \
{   \
  printf("constructor of %s is called for (%p).\n", STR(T), this);   \
}   \
~T() \
{   \
  printf("destructor of %s is called for (%p).\n", STR(T), this);   \
}

/// SDK-G
//  G的代码，编译时不依赖任何F的内容
class G
{
    //    通过回调函数来调用F中提供的功能(不引入F的文件、也不产生编译依赖)
    std::function<std::string(const std::string& name)> m_readFileFunc;

public:
    void setFunc(std::function<std::string(const std::string& name)> func)
    {
        m_readFileFunc = func;
    }
    DEBUG_PRINT_THIS(G);
};

/// SDK-F
class FConfig
{
public:
    //    FConfig的一些公用函数(可能来自于其他SDK)
    std::function<std::string(const std::string&)> _readFileFunc;
    std::shared_ptr<G> m_pG; // 出于某些需求，需要通过FConfig来传递G的对象。

    std::string readFile(const std::string& name)
    {
        assert(_readFileFunc);
        return _readFileFunc(name);
    }

    DEBUG_PRINT_THIS(FConfig);
};

class F
{
public:
    void createG(const std::shared_ptr<FConfig>& config)
    {
        assert(config.use_count() == 1); // 引用传递config，不产生拷贝、引用计数不更改
        m_pG = std::make_shared<G>();
        m_pG->setFunc(
            [&config](const std::string& name)->std::string
            {
              assert(config);
              return config->readFile(name);
            }
        );
        assert(config.use_count() == 1);
    }
    void init()
    {
        std::shared_ptr<FConfig> config = std::make_shared<FConfig>();
        //	此处省略对config的初始化
        assert(config.use_count() == 1);
        createG(config);
        assert(config.use_count() == 1);
        config->m_pG = m_pG;
        assert(m_pG.use_count() == 2);

        config.reset();
        assert(config == nullptr); // 实际未调用FConfig的析构函数。
        assert(m_pG.use_count() == 1); // pG引用计数应该为1、实际为2.
    }

    DEBUG_PRINT_THIS(F);
private:
    std::shared_ptr<G> m_pG;
};

/// APP
void lambdaErrorTest()
{
    auto f = std::make_shared<F>();
    f->init();
    f.reset(); // 此时，G和FConfig的析构函数没有调用
}

/*
以下为程序输出
constructor of F is called for (0x95de80).
constructor of FConfig is called for (0x95eec0).
constructor of G is called for (0x95ef10).
destructor of F is called for (0x95de80).
*/