//
// Created by william on 2023/12/11.
//
#include <iostream>
#include <json11/json11.hpp>
#include <string>

void jsonTest()
{
    const std::string jsonSrc =
        R"json(
                {
                "k1":"v1",
                "k2":42,
                "k3":["a",123,true,false,null]
                }
        )json";
    std::string err;
    const auto json = json11::Json::parse(jsonSrc, err);
    std::cout << "k1: " << json["k1"].string_value() << "\n";
    std::cout << "k2: " << json["k2"].int_value() << "\n";
    std::cout << "k3: " << json["k3"].dump() << "\n";
}