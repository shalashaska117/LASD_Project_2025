#include "test.hpp"

#include "../container/container.hpp"
#include "../container/dictionary.hpp"
#include "../container/linear.hpp"
#include "../container/mappable.hpp"
#include "../container/testable.hpp"
#include "../container/traversable.hpp"
#include "../vector/vector.hpp"
#include "../list/list.hpp"
#include "../set/lst/setlst.hpp"
#include "../set/vec/setvec.hpp"
#include "../set/set.hpp"


#include "../heap/heap.hpp"
#include "../heap/vec/heapvec.hpp"
#include "../pq/pq.hpp"
#include "../pq/heap/pqheap.hpp"

#include <iostream>
#include <string>

namespace lasd {

unsigned int totalTests = 0;
unsigned int failedTests = 0;

void Check(bool condition, const std::string& testName) {
    ++totalTests;
    if (condition) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
        ++failedTests;
    }
}

void TestResetCounter() {
    totalTests = 0;
    failedTests = 0;
}

void TestFullContainer() {
    std::cout << "\n==== Test FullContainer on Vector<int> ====\n" << std::endl;

    lasd::Vector<int> vec; // empty vector created internally

    // Test Container
    Check(vec.Size() >= 0, "Size() >= 0");
    Check((vec.Empty() && vec.Size() == 0) || (!vec.Empty() && vec.Size() > 0), "Size/Empty consistency");

    // Test ResizableContainer
    ulong newSize = 10;
    vec.Resize(newSize);
    Check(vec.Size() == newSize, "Resize(10) sets the Size correctly");

    // Test ClearableContainer
    vec.lasd::ResizableContainer::Clear();
    Check(vec.Size() == 0, "Clear() resets the Size to zero");
    Check(vec.Empty(), "Clear() makes the container empty");

    // Test access and modification
    vec.Resize(5);
    for (ulong i = 0; i < 5; ++i)
        vec[i] = i * 100;

    Check(vec[0] == 0, "vec[0] == 0");
    Check(vec[4] == 400, "vec[4] == 400");
}

void TestFullDictionaryInt() {
    std::cout << "\n==== Test Full DictionaryContainer<int> + OrderedDictionaryContainer<int> on SetVec ====\n" << std::endl;

    lasd::SetVec<int> dict;

    // === Insert / Remove ===
    Check(dict.Insert(10), "Insert(10)");
    Check(!dict.Insert(10), "Insert(10) duplicate");
    Check(dict.Insert(30), "Insert(30)");
    Check(dict.Insert(20), "Insert(20)");
    Check(dict.Insert(40), "Insert(40)");
    Check(dict.Remove(10), "Remove(10)");
    Check(!dict.Remove(99), "Remove(99) does not exist");

    // === InsertAll (TraversableContainer&)
    lasd::Vector<int> v1(3);
    v1[0] = 5; v1[1] = 15; v1[2] = 25;
    Check(dict.lasd::DictionaryContainer<int>::InsertAll(v1), "InsertAll(v1) - TraversableContainer");

    // === InsertAll (MappableContainer&&)
    lasd::Vector<int> v2(3);
    v2[0] = 26; v2[1] = 35; v2[2] = 45;
    Check(dict.lasd::DictionaryContainer<int>::InsertAll(std::move(v2)), "InsertAll(move(v2)) - MappableContainer");

    // === InsertSome (TraversableContainer&)
    lasd::Vector<int> v3(3);
    v3[0] = 15; v3[1] = 35; v3[2] = 50; // 15 and 35 are already present, 50 is new
    Check(dict.lasd::DictionaryContainer<int>::InsertSome(v3), "InsertSome(v3) - TraversableContainer");

    // === InsertSome (MappableContainer&&)
    lasd::Vector<int> v4(3);
    v4[0] = 25; v4[1] = 26; v4[2] = 60; // 25 and 26 duplicates, 60 new
    Check(dict.lasd::DictionaryContainer<int>::InsertSome(std::move(v4)), "InsertSome(move(v4)) - MappableContainer");

    // === RemoveAll / RemoveSome
    lasd::Vector<int> v5(3);
    v5[0] = 5; v5[1] = 15; v5[2] = 100; // 100 does not exist
    Check(!dict.lasd::DictionaryContainer<int>::RemoveAll(v5), "RemoveAll(v5) fails (100 does not exist)");

    dict.Insert(15); // Re-insert one to test RemoveSome
    Check(dict.lasd::DictionaryContainer<int>::RemoveSome(v5), "RemoveSome(v5) removes at least one");

    // === OrderedDictionary methods
    Check(dict.Min() == 20, "Min() == 20");
    Check(dict.Max() == 60, "Max() == 60");

    Check(dict.Predecessor(60) == 50, "Predecessor(60) == 50");
    Check(dict.Successor(30) == 35, "Successor(30) == 35");

    dict.RemoveMin(); // removes 20
    Check(dict.Min() == 25, "RemoveMin(), new Min == 25");

    dict.RemoveMax(); // removes 60
    Check(dict.Max() ==  50, "RemoveMax(), new Max == 50");

    Check(dict.PredecessorNRemove(50) == 45, "PredecessorNRemove(50) == 45");
    Check(dict.SuccessorNRemove(25) == 26, "SuccessorNRemove(25) == 26");

    // === Final clear
    dict.Clear();
    Check(dict.Empty(), "Clear() -> empty container");
}

void TestFullLinearContainerInt() {
    lasd::Vector<int> vec(5);
    for (ulong i = 0; i < 5; ++i)
        vec[i] = 5 - i; // 5, 4, 3, 2, 1

    // Accesso e informazioni base
    Check(vec.lasd::LinearContainer<int>::Size() == 5, "Size() == 5");
    Check(!vec.lasd::LinearContainer<int>::Empty(), "Empty() == false");
    Check(vec[0] == 5, "vec[0] == 5");
    Check(vec[4] == 1, "vec[4] == 1");
    Check(vec.lasd::LinearContainer<int>::Front() == 5, "Front() == 5");
    Check(vec.lasd::LinearContainer<int>::Back() == 1, "Back() == 1");

    // Modifications
    vec[0] = 99;
    vec[4] = 77;
    Check(vec[0] == 99, "vec[0] modified to 99");
    Check(vec[4] == 77, "vec[4] modified to 77");

    vec.lasd::MutableLinearContainer<int>::Front() = 100;
    vec.lasd::MutableLinearContainer<int>::Back() = 200;
    Check(vec[0] == 100, "Front() modified to 100");
    Check(vec[4] == 200, "Back() modified to 200");

    // Comparisons
    lasd::Vector<int> same(5);
    same[0] = 100; same[1] = 4; same[2] = 3; same[3] = 2; same[4] = 200;
    Check(vec.lasd::LinearContainer<int>::operator==(same), "operator== on equal contents");

    same[2] = 300;
    Check(vec.lasd::LinearContainer<int>::operator!=(same), "operator!= on different contents");

    lasd::Vector<int> shorter(3);
    shorter[0] = 100; shorter[1] = 4; shorter[2] = 3;
    Check(vec.lasd::LinearContainer<int>::operator!=(shorter), "operator!= on different size");

    // Traverse
    int acc = 0;
    vec.lasd::LinearContainer<int>::Traverse([&acc](const int& x) { acc += x; });
    Check(acc > 0, "Traverse() executed");

    acc = 0;
    vec.lasd::LinearContainer<int>::PreOrderTraverse([&acc](const int& x) { acc += x; });
    Check(acc > 0, "PreOrderTraverse() executed");

    acc = 0;
    vec.lasd::LinearContainer<int>::PostOrderTraverse([&acc](const int& x) { acc += x; });
    Check(acc > 0, "PostOrderTraverse() executed");

    // Map
    vec.lasd::MutableLinearContainer<int>::Map([](int& x) { x += 1; });
    Check(vec[0] == 101, "Map() applied (vec[0] == 101)");

    vec.lasd::MutableLinearContainer<int>::PreOrderMap([](int& x) { x *= 2; });
    Check(vec[0] == 202, "PreOrderMap() applied (vec[0] *= 2)");

    vec.lasd::MutableLinearContainer<int>::PostOrderMap([](int& x) { x -= 1; });
    Check(vec[4] == 401, "PostOrderMap() applied (vec[4] -= 1)");

    // Sort
    lasd::SortableVector<int> svec(5);
    svec[0] = 9;
    svec[1] = 3;
    svec[2] = 7;
    svec[3] = 1;
    svec[4] = 5;
    
    svec.Sort();
    for (ulong i = 0; i < svec.Size() - 1; ++i)
        Check(svec[i] <= svec[i + 1], "Sort() ordered correctly");
}

void TestVectorInt() {
    std::cout << "==== Test Vector<int> ====" << std::endl;

    Vector<int> vec(3);
    Check(vec.Size() == 3, "Constructor Vector(3)");
    Check(!vec.Empty(), "Empty() == false");

    vec[0] = 10;
    vec[1] = 20;
    vec[2] = -5;
    
    Check(vec[0] == 10, "operator[] access 0");
    Check(vec[1] == 20, "operator[] access 1");
    Check(vec[2] == -5, "operator[] access 2");

    Check(vec.Front() == 10, "Front() == 10");
    Check(vec.Back() == -5, "Back() == -5");

    Check(vec.Exists(20), "Exists(20) == true");
    Check(!vec.Exists(100), "Exists(100) == false");

    // Fold: generic sum
    auto foldAdd = [](const int& val, const int& acc) {
        return acc + val;
    };
    int accFold = vec.Fold<int>(foldAdd, 0);
    Check(accFold == 25, "Fold<int> generic sum [Vector<int>]");

    // PreOrderFold: sum
    auto preOrderAdd = [](const int& val, const int& acc) {
        return acc + val;
    };
    int accPre = vec.PreOrderFold<int>(preOrderAdd, 0);
    Check(accPre == 25, "PreOrderFold<int> sum [Vector<int>]");

    // PostOrderFold: product
    auto postOrderMul = [](const int& val, const int& acc) {
        return acc * val;
    };
    int accPost = vec.PostOrderFold<int>(postOrderMul, 1);
    Check(accPost == -1000, "PostOrderFold<int> product [Vector<int>]");

    // Traverse: sum
    int sumTraverse = 0;
    vec.Traverse([&](const int& val) { sumTraverse += val; });
    Check(sumTraverse == 25, "Traverse sum [Vector<int>]");


    // PreOrderTraverse: sum
    int sum = 0;
    vec.PreOrderTraverse([&](const int& val) { sum += val; });
    Check(sum == 25, "PreOrderTraverse sum == 25");

    // PostOrderTraverse: multiplication
    int prod = 1;
    vec.PostOrderTraverse([&](const int& val) { prod *= val; });
    Check(prod == 10 * 20 * -5, "PostOrderTraverse product == -1000");

    // Map: doubles each value
    vec.Map([](int& val) { val *= 2; });
    Check(vec[0] == 20 && vec[1] == 40 && vec[2] == -10, "Map value doubling");

    // PreOrderMap: sum in order
    int sumPreMap = 0;
    vec.PreOrderMap([&](int& val) {
        sumPreMap += val;
    });
    Check(sumPreMap == 50, "PreOrderMap sum after Map");

    // PostOrderMap
    int orderTracker = 0;
    vec.PostOrderMap([&](int& val) {
        val = ++orderTracker;
    });
    Check(vec[0] == 3 && vec[1] == 2 && vec[2] == 1, "PostOrderMap reverses the write order");

    // Sort test (usa SortableVector)
    SortableVector<int> sv(5);
    sv[0] = 4;
    sv[1] = 7;
    sv[2] = 2;
    sv[3] = -1;
    sv[4] = 9;

    sv.Sort(); // [-1, 2, 4, 7, 9]
    Check(sv[0] == -1 && sv[4] == 9, "SortableVector::Sort()");

    // Copy constructor
    Vector<int> copy(sv);
    Check(copy == sv, "Copy constructor + operator==");

    // Modify and check !=
    copy[2] = 99;
    Check(copy != sv, "operator!= after modification");

    // Copy assignment
    Vector<int> assign;
    assign = sv;
    Check(assign == sv, "copy operator=");

    // Move constructor
    Vector<int> moved(std::move(copy));
    Check(moved.Size() == 5 && copy.Size() == 0, "Move constructor");

    // Move assignment
    Vector<int> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 5 && assign.Size() == 0, "move operator=");

    // Resize and Clear
    movedAssign.Resize(10);
    Check(movedAssign.Size() == 10, "Resize(10)");

    movedAssign.Resize(2);
    Check(movedAssign.Size() == 2, "Resize(2)");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() e Empty()");

    std::cout << std::endl;
}

void TestVectorFloat() {
    std::cout << "==== Test Vector<float> ====" << std::endl;

    auto IsClose = [](float a, float b, float tol = 1e-6f) -> bool {
        float diff = a - b;
        if (diff < 0) diff = -diff;
        return diff < tol;
    };

    // Constructor and initialization with mixed values
    SortableVector<float> vec(3);
    vec[0] = -2.5f;
    vec[1] = 4.0f;
    vec[2] = -1.0f;

    Check(vec.Size() == 3, "Constructor SortableVector(3) [float]");
    Check(!vec.Empty(), "Empty() == false [float]");

    Check(IsClose(vec[0], -2.5f), "operator[] negative access [float]");
    Check(IsClose(vec[1], 4.0f), "operator[] positive access [float]");
    Check(IsClose(vec[2], -1.0f), "operator[] negative access [float]");

    Check(IsClose(vec.Front(), -2.5f), "Front() [float]");
    Check(IsClose(vec.Back(), -1.0f), "Back() [float]");

    Check(vec.Exists(4.0f), "Exists(4.0) == true [float]");
    Check(vec.Exists(-2.5f), "Exists(-2.5) == true [float]");
    Check(!vec.Exists(9.9f), "Exists(9.9) == false [float]");

    // Fold: classic sum
    auto foldAdd = [](const float& val, const float& acc) {
        return acc + val;
    };
    float accFold = vec.Fold<float>(foldAdd, 0.0f);
    Check(IsClose(accFold, 0.5f), "Fold<float> sum [Vector<float>]");

    // PreOrderFold: sum in forward order
    auto preOrderAdd = [](const float& val, const float& acc) {
        return acc + val;
    };
    float accPre = vec.PreOrderFold<float>(preOrderAdd, 0.0f);
    Check(IsClose(accPre, 0.5f), "PreOrderFold<float> sum [Vector<float>]");

    // PostOrderFold: product in reverse order
    auto postOrderMul = [](const float& val, const float& acc) {
        return acc * val;
    };
    float accPost = vec.PostOrderFold<float>(postOrderMul, 1.0f);
    Check(IsClose(accPost, 10.0f), "PostOrderFold<float> product [Vector<float>]");

    // Fold sum (PreOrder)
    float sum = 0;
    vec.PreOrderTraverse([&](const float& val) { sum += val; });
    Check(IsClose(sum, 0.5f), "PreOrderTraverse sum [float]");

    // Fold product (PostOrder)
    float prod = 1.0f;
    vec.PostOrderTraverse([&](const float& val) { prod *= val; });
    Check(IsClose(prod, -2.5f * 4.0f * -1.0f), "PostOrderTraverse product [float]");

    // Map: add 0.5
    vec.Map([](float& val) { val += 0.5f; });
    Check(IsClose(vec[0], -2.0f) && IsClose(vec[1], 4.5f) && IsClose(vec[2], -0.5f), "Map increment [float]");

    // PreOrderMap: sum in forward order
    float preOrderSum = 0.0f;
    vec.PreOrderMap([&](float& val) {
        preOrderSum += val;
        val += 1.0f;  // to modify the values traceably
    });
    Check(IsClose(preOrderSum, 2.0f), "PreOrderMap sum [float]");

    // PostOrderMap
    vec.PostOrderMap([](float& val) {
        val *= -1.0f;
    });
    Check(IsClose(vec[0], 1.0f) && IsClose(vec[1], -5.5f) && IsClose(vec[2], -0.5f), "PostOrderMap negation [float]");

    // Sort
    vec.Sort(); // [-5.5, -0.5, 1.0]
    Check(IsClose(vec[0], -5.5f) && IsClose(vec[2], 1.0f), "Sort with negatives [float]");

    // Copy constructor
    Vector<float> copy(vec);
    Check(copy == vec, "Copy constructor + operator== [float]");

    copy[1] = 123.456f;
    Check(copy != vec, "operator!= after modification [float]");

    // Copy assignment
    Vector<float> assign;
    assign = vec;
    Check(assign == vec, "copy operator= [float]");

    // Move constructor
    Vector<float> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor [float]");

    // Move assignment
    Vector<float> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator= [float]");

    // Resize and Clear
    movedAssign.Resize(5);
    Check(movedAssign.Size() == 5, "Resize(5) [float]");

    movedAssign.Resize(2);
    Check(movedAssign.Size() == 2, "Resize(2) [float]");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [float]");

    std::cout << std::endl;
}

void TestVectorDouble() {
    std::cout << "==== Test Vector<double> ====" << std::endl;

    auto IsClose = [](double a, double b, double tol = 1e-10) -> bool {
        double diff = a - b;
        if (diff < 0) diff = -diff;
        return diff < tol;
    };

    // Constructor from size and initialization with negatives
    SortableVector<double> vec(3);
    vec[0] = -5.5;
    vec[1] = 3.3;
    vec[2] = -1.1;

    Check(vec.Size() == 3, "Constructor SortableVector(3)");
    Check(!vec.Empty(), "Empty() == false");

    Check(IsClose(vec[0], -5.5), "operator[] negative access");
    Check(IsClose(vec[1], 3.3), "operator[] positive access");
    Check(IsClose(vec[2], -1.1), "operator[] negative access");

    Check(IsClose(vec.Front(), -5.5), "Front() == -5.5");
    Check(IsClose(vec.Back(), -1.1), "Back() == -1.1");

    Check(vec.Exists(3.3), "Exists(3.3) == true");
    Check(vec.Exists(-5.5), "Exists(-5.5) == true");
    Check(!vec.Exists(100.0), "Exists(100.0) == false");

    // Fold: generic sum
    auto foldAdd = [](const double& val, const double& acc) {
        return acc + val;
    };
    double accFold = vec.Fold<double>(foldAdd, 0.0);
    Check(IsClose(accFold, -3.3), "Fold<double> generic sum [Vector<double>]");

    // PreOrderFold: sum
    auto preOrderAdd = [](const double& val, const double& acc) {
        return acc + val;
    };
    double accPre = vec.PreOrderFold<double>(preOrderAdd, 0.0);
    Check(IsClose(accPre, -3.3), "PreOrderFold<double> sum [Vector<double>]");

    // PostOrderFold: product
    auto postOrderMul = [](const double& val, const double& acc) {
        return acc * val;
    };
    double accPost = vec.PostOrderFold<double>(postOrderMul, 1.0);
    Check(IsClose(accPost, -5.5 * 3.3 * -1.1), "PostOrderFold<double> product [Vector<double>]");

    // Traverse: sum
    double sumTraverse = 0.0;
    vec.Traverse([&](const double& val) { sumTraverse += val; });
    Check(IsClose(sumTraverse, -3.3), "Traverse sum [Vector<double>]");

    // Traverse: sum
    double sum = 0;
    vec.PreOrderTraverse([&](const double& val) { sum += val; });
    Check(IsClose(sum, -3.3), "PreOrderTraverse sum == -3.3");

    // Traverse: product
    double product = 1.0;
    vec.PostOrderTraverse([&](const double& val) { product *= val; });
    Check(IsClose(product, -5.5 * 3.3 * -1.1), "PostOrderTraverse product == 19.965");

    // Map: add 1.0 to each element
    vec.Map([](double& val) { val += 1.0; }); // -5.5 -> -4.5, etc.
    Check(IsClose(vec[0], -4.5) && IsClose(vec[1], 4.3) && IsClose(vec[2], -0.1), "Map increment +1.0");

    // PreOrderMap: sum of the values and increment +2.0
    double preOrderSum = 0.0;
    vec.PreOrderMap([&](double& val) {
        preOrderSum += val;
        val += 2.0;
    });
    Check(IsClose(preOrderSum, -0.3), "PreOrderMap sum");

    // PostOrderMap: multiplication by 10.0 in reverse order
    vec.PostOrderMap([](double& val) {
        val *= 10.0;
    });
    Check(IsClose(vec[0], -25.0) && IsClose(vec[1], 63.0) && IsClose(vec[2], 19.0), "PostOrderMap multiply x10");

    // Sort
    vec.Sort(); // [-25.0, 19.0, 63.0]
    Check(IsClose(vec[0], -25.0) && IsClose(vec[1], 19.0) && IsClose(vec[2], 63.0), "Sort with negatives");

    // Copy constructor
    Vector<double> copy(vec);
    Check(copy == vec, "Copy constructor and operator==");

    copy[1] = 999.9;
    Check(copy != vec, "operator!= after modification");

    // Copy assignment
    Vector<double> assign;
    assign = vec;
    Check(assign == vec, "Copy assignment");

    // Move constructor
    Vector<double> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor");

    // Move assignment
    Vector<double> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "Move assignment");

    // Resize / Clear
    movedAssign.Resize(6);
    Check(movedAssign.Size() == 6, "Resize(6)");
    movedAssign.Resize(2);
    Check(movedAssign.Size() == 2, "Resize(2)");
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() e Empty()");

    std::cout << std::endl;
}

void TestVectorString() {
    std::cout << "==== Test Vector<std::string> ====" << std::endl;

    // Constructor and insertion
    SortableVector<std::string> vec(3);
    vec[0] = "banana";
    vec[1] = "Apple";
    vec[2] = "";

    Check(vec.Size() == 3, "Constructor SortableVector(3) [string]");
    Check(!vec.Empty(), "Empty() == false [string]");

    Check(vec[0] == "banana", "operator[] access 0 [string]");
    Check(vec[1] == "Apple", "operator[] access 1 [string]");
    Check(vec[2] == "", "operator[] access 2 (empty) [string]");

    Check(vec.Front() == "banana", "Front() == 'banana'");
    Check(vec.Back() == "", "Back() == '' (empty)");

    Check(vec.Exists("banana"), "Exists('banana') == true");
    Check(vec.Exists(""), "Exists('') == true");
    Check(!vec.Exists("kiwi"), "Exists('kiwi') == false");

    // Fold: concatenation
    auto foldConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accFold = vec.Fold<std::string>(foldConcat, "");
    Check(accFold == "bananaApple", "Fold<string> concatenation [Vector<string>]");

    // PreOrderFold: normal concatenation
    auto preOrderConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accPre = vec.PreOrderFold<std::string>(preOrderConcat, "");
    Check(accPre == "bananaApple", "PreOrderFold<string> concatenation [Vector<string>]");

    // PostOrderFold: reverse concatenation
    auto postOrderConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accPost = vec.PostOrderFold<std::string>(postOrderConcat, "");
    Check(accPost == "Applebanana", "PostOrderFold<string> reverse concatenation [Vector<string>]");

    // Traverse: sum of lengths
    ulong totalLen = 0;
    vec.Traverse([&](const std::string& s) { totalLen += s.length(); });
    Check(totalLen == 11, "Traverse sum of lengths [Vector<string>]");

    // Traverse: sum of total length
    totalLen = 0;
    vec.PreOrderTraverse([&](const std::string& s) { totalLen += s.size(); });
    Check(totalLen == 6 + 5 + 0, "PreOrderTraverse sum of lengths == 11");

    // Traverse: reverse concatenation
    std::string result;
    vec.PostOrderTraverse([&](const std::string& s) { result += s; });
    Check(result == std::string("") + "Apple" + "banana", "PostOrderTraverse reverse concatenation");

    // Map: add "!"
    vec.Map([](std::string& s) { s += "!"; });
    Check(vec[0] == "banana!" && vec[1] == "Apple!" && vec[2] == "!", "Map adds '!'");

    // PreOrderMap: adds prefix "#"
    vec.PreOrderMap([](std::string& s) {
        s = "#" + s;
    });

    // PostOrderMap: adds suffix "?"
    vec.PostOrderMap([](std::string& s) {
        s += "?";
    });

    // Sort (lexicographic)
    vec.Sort(); // → "#!?", "#Apple!?", "#banana!?"
    Check(vec[0] == "#!?" && vec[1] == "#Apple!?" && vec[2] == "#banana!?", "Alphabetical sort");

    // Copy constructor
    Vector<std::string> copy(vec);
    Check(copy == vec, "Copy constructor + operator== [string]");

    copy[2] = "ZZZ";
    Check(copy != vec, "operator!= after modification [string]");

    // Copy assignment
    Vector<std::string> assign;
    assign = vec;
    Check(assign == vec, "copy operator= [string]");

    // Move constructor
    Vector<std::string> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor [string]");

    // Move assignment
    Vector<std::string> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator= [string]");

    // Resize / Clear
    movedAssign.Resize(5);
    Check(movedAssign.Size() == 5, "Resize(5) [string]");

    movedAssign.Resize(2);
    Check(movedAssign.Size() == 2, "Resize(2) [string]");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [string]");

    std::cout << std::endl;
}

void TestListInt() {
    std::cout << "==== Test List<int> ====" << std::endl;

    List<int> list;
    Check(list.Empty(), "empty List after constructor");
    Check(list.Size() == 0, "Size() == 0 after constructor");

    // Insertions
    list.InsertAtFront(3);
    list.InsertAtBack(7);
    list.InsertAtFront(-1);
    list.InsertAtBack(10); // List: -1, 3, 7, 10
    Check(list.Size() == 4, "Size() == 4 after insert");
    Check(list.Front() == -1, "Front() == -1");
    Check(list.Back() == 10, "Back() == 10");

    // Exists e accessi
    Check(list.Exists(3), "Exists(3) == true");
    Check(!list.Exists(999), "Exists(999) == false");
    Check(list[1] == 3, "operator[] == 3");

    // Fold: generic sum
    auto foldAdd = [](const int& val, const int& acc) {
        return acc + val;
    };
    int accFold = list.Fold<int>(foldAdd, 0);
    Check(accFold == 19, "Fold<int> generic sum [List<int>]");

    // PreOrderFold: sum
    auto preOrderAdd = [](const int& val, const int& acc) {
        return acc + val;
    };
    int accPre = list.PreOrderFold<int>(preOrderAdd, 0);
    Check(accPre == 19, "PreOrderFold<int> sum [List<int>]");

    // PostOrderFold: product
    auto postOrderMul = [](const int& val, const int& acc) {
        return acc * val;
    };
    int accPost = list.PostOrderFold<int>(postOrderMul, 1);
    Check(accPost == -210, "PostOrderFold<int> product [List<int>]");

    // Traverse: sum
    int sumTraverse = 0;
    list.Traverse([&](const int& val) { sumTraverse += val; });
    Check(sumTraverse == 19, "Traverse sum [List<int>]");

    // Traverse: sum
    int sum = 0;
    list.Traverse([&](const int& x) { sum += x; }); // -1 + 3 + 7 + 10 = 19
    Check(sum == 19, "Traverse() sum == 19");

    // PostOrderTraverse: product
    int prod = 1;
    list.PostOrderTraverse([&](const int& x) { prod *= x; });
    Check(prod == -1 * 3 * 7 * 10, "PostOrderTraverse product == -210");

    // Map: doubling
    list.Map([](int& x) { x *= 2; }); // -2, 6, 14, 20
    Check(list[0] == -2 && list[3] == 20, "Map value doubling");

    // PreOrderMap: increment
    list.PreOrderMap([](int& x) { x += 1; }); // -1, 7, 15, 21
    Check(list[1] == 7, "PreOrderMap increment");

    // PostOrderMap: decrement
    list.PostOrderMap([](int& x) { x -= 2; }); // -3, 5, 13, 19
    Check(list.Back() == 19, "PostOrderMap decrement");

    // FrontNRemove / BackNRemove
    int front = list.FrontNRemove(); // -3
    int back = list.BackNRemove();   // 19
    Check(front == -3 && back == 19, "FrontNRemove == -3, BackNRemove == 19");
    Check(list.Size() == 2, "Size() == 2 after NRemove");

    // RemoveFromFront / Back
    list.RemoveFromFront(); // 5
    list.RemoveFromBack();  // 13
    Check(list.Empty(), "empty list after all removals");

    // Copy constructor
    list.InsertAtBack(1);
    list.InsertAtBack(2);
    list.InsertAtBack(3);
    List<int> copy(list);
    Check(copy == list, "Copy constructor + operator==");

    copy[1] = 99;
    Check(copy != list, "operator!= after modification");

    // Move constructor
    List<int> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor");

    // Copy assignment
    List<int> assign;
    assign = list;
    Check(assign == list, "copy operator=");

    // Move assignment
    List<int> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator=");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [List<int>]");

    std::cout << std::endl;
}

void TestListFloat() {
    std::cout << "==== Test List<float> ====" << std::endl;

    auto IsClose = [](float a, float b, float tol = 1e-6f) -> bool {
        float diff = a - b;
        if (diff < 0) diff = -diff;
        return diff < tol;
    };

    List<float> list;
    Check(list.Empty(), "empty List after constructor");
    Check(list.Size() == 0, "Size() == 0 after constructor");

    // Insertions
    list.InsertAtFront(-1.5f);
    list.InsertAtBack(3.0f);
    list.InsertAtFront(0.5f);
    list.InsertAtBack(-2.0f); // List: 0.5, -1.5, 3.0, -2.0

    Check(list.Size() == 4, "Size() == 4 after insert");
    Check(IsClose(list.Front(), 0.5f), "Front() == 0.5");
    Check(IsClose(list.Back(), -2.0f), "Back() == -2.0");

    // Exists e operator[]
    Check(list.Exists(-1.5f), "Exists(-1.5) == true");
    Check(!list.Exists(10.0f), "Exists(10.0) == false");
    Check(IsClose(list[2], 3.0f), "operator[] == 3.0");

    // Fold: sum
    auto foldAdd = [](const float& val, const float& acc) {
        return acc + val;
    };
    float accFold = list.Fold<float>(foldAdd, 0.0f);
    Check(IsClose(accFold, 0.0f), "Fold<float> sum [List<float>]");

    // PreOrderFold: sum
    auto preOrderAdd = [](const float& val, const float& acc) {
        return acc + val;
    };
    float accPre = list.PreOrderFold<float>(preOrderAdd, 0.0f);
    Check(IsClose(accPre, 0.0f), "PreOrderFold<float> sum [List<float>]");

    // PostOrderFold: product
    auto postOrderMul = [](const float& val, const float& acc) {
        return acc * val;
    };
    float accPost = list.PostOrderFold<float>(postOrderMul, 1.0f);
    Check(IsClose(accPost, 4.5f), "PostOrderFold<float> product [List<float>]");

    // Traverse: sum
    float sumTraverse = 0.0f;
    list.Traverse([&](const float& val) { sumTraverse += val; });
    Check(IsClose(sumTraverse, 0.0f), "Traverse sum [List<float>]");

    // Traverse sum
    float sum = 0.0f;
    list.PreOrderTraverse([&](const float& x) { sum += x; });
    Check(IsClose(sum, 0.5f + (-1.5f) + 3.0f + (-2.0f)), "PreOrderTraverse sum");

    // Traverse product
    float prod = 1.0f;
    list.PostOrderTraverse([&](const float& x) { prod *= x; });
    Check(IsClose(prod, 0.5f * -1.5f * 3.0f * -2.0f), "PostOrderTraverse product");

    // Map: +1.0
    list.Map([](float& x) { x += 1.0f; }); // 1.5, -0.5, 4.0, -1.0
    Check(IsClose(list[0], 1.5f) && IsClose(list[3], -1.0f), "Map increment");

    // PreOrderMap: x2
    list.PreOrderMap([](float& x) { x *= 2; }); // 3.0, -1.0, 8.0, -2.0
    Check(IsClose(list[1], -1.0f), "PreOrderMap doubling");

    // PostOrderMap: -1
    list.PostOrderMap([](float& x) { x -= 1.0f; }); // 2.0, -2.0, 7.0, -3.0
    Check(IsClose(list.Back(), -3.0f), "PostOrderMap decrement");

    // FrontNRemove / BackNRemove
    float front = list.FrontNRemove(); // 2.0
    float back = list.BackNRemove();   // -3.0
    Check(IsClose(front, 2.0f) && IsClose(back, -3.0f), "FrontNRemove e BackNRemove");
    Check(list.Size() == 2, "Size() == 2 after NRemove");

    // RemoveFromFront / Back
    list.RemoveFromFront(); // -2.0
    list.RemoveFromBack();  // 7.0
    Check(list.Empty(), "empty list after all removals");

    // Copy constructor
    list.InsertAtBack(1.1f);
    list.InsertAtBack(2.2f);
    list.InsertAtBack(-3.3f);
    List<float> copy(list);
    Check(copy == list, "Copy constructor + operator== [float]");

    copy[1] = 99.9f;
    Check(copy != list, "operator!= after modification [float]");

    // Move constructor
    List<float> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor [float]");

    // Copy assignment
    List<float> assign;
    assign = list;
    Check(assign == list, "copy operator= [float]");

    // Move assignment
    List<float> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator= [float]");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [float]");

    std::cout << std::endl;
}

void TestListDouble() {
    std::cout << "==== Test List<double> ====" << std::endl;

    auto IsClose = [](double a, double b, double tol = 1e-10) -> bool {
        double diff = a - b;
        if (diff < 0) diff = -diff;
        return diff < tol;
    };

    List<double> list;
    Check(list.Empty(), "empty List after constructor");
    Check(list.Size() == 0, "Size() == 0 after constructor");

    // Insertions
    list.InsertAtFront(-2.5);
    list.InsertAtBack(4.0);
    list.InsertAtFront(1.0);
    list.InsertAtBack(-3.5); // List: 1.0, -2.5, 4.0, -3.5

    Check(list.Size() == 4, "Size() == 4 after insert");
    Check(IsClose(list.Front(), 1.0), "Front() == 1.0");
    Check(IsClose(list.Back(), -3.5), "Back() == -3.5");

    Check(list.Exists(-2.5), "Exists(-2.5) == true");
    Check(!list.Exists(100.0), "Exists(100.0) == false");
    Check(IsClose(list[2], 4.0), "operator[] == 4.0");

    // Fold: sum
    auto foldAdd = [](const double& val, const double& acc) {
        return acc + val;
    };
    double accFold = list.Fold<double>(foldAdd, 0.0);
    Check(IsClose(accFold, -1.0), "Fold<double> sum [List<double>]");

    // PreOrderFold: sum
    auto preOrderAdd = [](const double& val, const double& acc) {
        return acc + val;
    };
    double accPre = list.PreOrderFold<double>(preOrderAdd, 0.0);
    Check(IsClose(accPre, -1.0), "PreOrderFold<double> sum [List<double>]");

    // PostOrderFold: product
    auto postOrderMul = [](const double& val, const double& acc) {
        return acc * val;
    };
    double accPost = list.PostOrderFold<double>(postOrderMul, 1.0);
    Check(IsClose(accPost, 35.0), "PostOrderFold<double> product [List<double>]");

    // Traverse: sum
    double sumTraverse = 0.0;
    list.Traverse([&](const double& val) { sumTraverse += val; });
    Check(IsClose(sumTraverse, -1.0), "Traverse sum [List<double>]");

    // Traverse: sum
    double sum = 0;
    list.PreOrderTraverse([&](const double& x) { sum += x; });
    Check(IsClose(sum, -1.0), "PreOrderTraverse sum == -1.0");

    // Traverse: product
    double prod = 1.0;
    list.PostOrderTraverse([&](const double& x) { prod *= x; });
    Check(IsClose(prod, 1.0 * -2.5 * 4.0 * -3.5), "PostOrderTraverse product");

    // Map: add 2.0
    list.Map([](double& x) { x += 2.0; }); // 3.0, -0.5, 6.0, -1.5
    Check(IsClose(list[0], 3.0) && IsClose(list[3], -1.5), "Map increment +2.0");

    // PreOrderMap: multiply * 2
    list.PreOrderMap([](double& x) { x *= 2.0; }); // 6.0, -1.0, 12.0, -3.0
    Check(IsClose(list[2], 12.0), "PreOrderMap doubling");

    // PostOrderMap: -1
    list.PostOrderMap([](double& x) { x -= 1.0; }); // 5.0, -2.0, 11.0, -4.0
    Check(IsClose(list.Back(), -4.0), "PostOrderMap decrement");

    // FrontNRemove / BackNRemove
    double front = list.FrontNRemove(); // 5.0
    double back = list.BackNRemove();   // -4.0
    Check(IsClose(front, 5.0) && IsClose(back, -4.0), "FrontNRemove e BackNRemove");
    Check(list.Size() == 2, "Size() == 2 after NRemove");

    // RemoveFromFront / Back
    list.RemoveFromFront(); // -2.0
    list.RemoveFromBack();  // 11.0
    Check(list.Empty(), "empty list after all removals");

    // Copy constructor
    list.InsertAtBack(3.14);
    list.InsertAtBack(-1.41);
    list.InsertAtBack(2.71);
    List<double> copy(list);
    Check(copy == list, "Copy constructor + operator== [double]");

    copy[1] = 99.99;
    Check(copy != list, "operator!= after modification [double]");

    // Move constructor
    List<double> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor [double]");

    // Copy assignment
    List<double> assign;
    assign = list;
    Check(assign == list, "copy operator= [double]");

    // Move assignment
    List<double> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator= [double]");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [double]");

    std::cout << std::endl;
}

void TestListString() {
    std::cout << "==== Test List<std::string> ====" << std::endl;

    List<std::string> list;
    Check(list.Empty(), "empty List after constructor");
    Check(list.Size() == 0, "Size() == 0 after constructor");

    // Insertions
    list.InsertAtFront("apple");
    list.InsertAtBack("banana");
    list.InsertAtFront("");
    list.InsertAtBack("APPLE"); // ["", "apple", "banana", "APPLE"]

    Check(list.Size() == 4, "Size() == 4 after insert");
    Check(list.Front() == "", "Front() == \"\"");
    Check(list.Back() == "APPLE", "Back() == \"APPLE\"");

    Check(list.Exists("banana"), "Exists(\"banana\") == true");
    Check(list.Exists(""), "Exists(\"\") == true");
    Check(!list.Exists("kiwi"), "Exists(\"kiwi\") == false");

    Check(list[1] == "apple", "operator[] == \"apple\"");

    // Fold: concatenation
    auto foldConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accFold = list.Fold<std::string>(foldConcat, "");
    Check(accFold == "applebananaAPPLE", "Fold<string> concatenation [List<string>]");

    // PreOrderFold: concatenation
    auto preOrderConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accPre = list.PreOrderFold<std::string>(preOrderConcat, "");
    Check(accPre == "applebananaAPPLE", "PreOrderFold<string> concatenation [List<string>]");

    // PostOrderFold: reverse concatenation
    auto postOrderConcat = [](const std::string& val, const std::string& acc) {
        return acc + val;
    };
    std::string accPost = list.PostOrderFold<std::string>(postOrderConcat, "");
    Check(accPost == "APPLEbananaapple", "PostOrderFold<string> reverse concatenation [List<string>]");

    // Traverse: sum of lengths
    ulong totalLen = 0;
    list.Traverse([&](const std::string& s) { totalLen += s.length(); });
    Check(totalLen == 16, "Traverse sum of lengths [List<string>]");

    // Traverse: sum of lengths
    totalLen = 0;
    list.PreOrderTraverse([&](const std::string& s) { totalLen += s.length(); });
    Check(totalLen == 5 + 6 + 5, "PreOrderTraverse sum of lengths == 16");

    // Traverse: reverse concatenation
    std::string reverseConcat;
    list.PostOrderTraverse([&](const std::string& s) { reverseConcat += s; });
    Check(reverseConcat == "APPLEbananaapple", "PostOrderTraverse concatenation");

    // Map: add exclamation mark
    list.Map([](std::string& s) { s += "!"; });
    Check(list[0] == "!" && list[3] == "APPLE!", "Map adds '!'");

    // PreOrderMap: uppercase
    list.PreOrderMap([](std::string& s) {
        for (char& c : s) c = std::toupper(c);
    });
    Check(list[1] == "APPLE!", "PreOrderMap uppercase");

    // PostOrderMap: trims the last 2 characters
    list.PostOrderMap([](std::string& s) {
        if (s.size() >= 2) s.erase(s.size() - 2);
    });
    Check(list.Back() == "APPL", "PostOrderMap final trim");

    // FrontNRemove / BackNRemove
    std::string f = list.FrontNRemove(); // "!"
    std::string b = list.BackNRemove();  // "APPL"
    Check(f == "!" && b == "APPL", "FrontNRemove/BackNRemove");
    Check(list.Size() == 2, "Size == 2 after NRemove");

    // RemoveFromFront / Back
    list.RemoveFromFront(); // "APPLE"
    list.RemoveFromBack();  // "BANAN"
    Check(list.Empty(), "empty list after all removals");

    // Copy constructor
    list.InsertAtBack("one");
    list.InsertAtBack("two");
    list.InsertAtBack("three");
    List<std::string> copy(list);
    Check(copy == list, "Copy constructor + operator==");

    copy[1] = "other";
    Check(copy != list, "operator!= after modification");

    // Move constructor
    List<std::string> moved(std::move(copy));
    Check(moved.Size() == 3 && copy.Size() == 0, "Move constructor");

    // Copy assignment
    List<std::string> assign;
    assign = list;
    Check(assign == list, "copy operator=");

    // Move assignment
    List<std::string> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 3 && assign.Size() == 0, "move operator=");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() [string]");

    std::cout << std::endl;
}

void TestSetLstInt() {
    std::cout << "==== Test SetLst<int> ====" << std::endl;

    lasd::SetLst<int> set;

    // More extensive Insert
    Check(set.Insert(5), "Insert(5)");
    Check(set.Insert(-10), "Insert(-10)");
    Check(set.Insert(-20), "Insert(-20)");
    Check(set.Insert(-3), "Insert(-3)");
    Check(set.Insert(0), "Insert(0)");
    Check(set.Insert(3), "Insert(3)");
    Check(set.Insert(10), "Insert(10)");
    Check(set.Insert(15), "Insert(15)");
    Check(set.Insert(-30), "Insert(-30)");
    Check(set.Insert(8), "Insert(8)");
    Check(set.Insert(1), "Insert(1)");
    Check(set.Insert(-15), "Insert(-15)");
    Check(set.Insert(20), "Insert(20)");
    Check(!set.Insert(3), "Insert(3) duplicato");

    // Insert rvalue
    int val = 7;
    Check(set.Insert(std::move(val)), "Insert(7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30 && set[1] == -20, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10), "Exists(-10)");
    Check(!set.Exists(42), "Exists(42) == false");
    Check(set.Remove(3), "Remove(3)");
    Check(!set.Remove(3), "Remove(3) already removed");
    
    // ===== Traverse =====
    int traverseSum = 0;
    set.Traverse([&](const int& val) {
        traverseSum += val;
    });
    Check(traverseSum == -12, "Traverse sum");

    // ===== PreOrderTraverse =====
    int preSum = 0;
    set.PreOrderTraverse([&](const int& val) {
        preSum += val;
    });
    Check(preSum == -12, "PreOrderTraverse sum");

    // ===== PostOrderTraverse =====
    int postSum = 0;
    set.PostOrderTraverse([&](const int& val) {
        postSum += val;
    });
    Check(postSum == -12, "PostOrderTraverse sum");

    // Fold
    int folded = set.Fold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(folded == -12, "Fold sum");

    // PreOrderFold 
    int preFolded = set.PreOrderFold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(preFolded == -12, "PreOrderFold sum");

    // PostOrderFold
    int postFolded = set.PostOrderFold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(postFolded == -12, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30, "Min()");
    Check(set.Max() == 20, "Max()");

    // Predecessor
    Check(set.Predecessor(1) == 0, "Predecessor(1)");
    Check(set.Predecessor(-10) == -15, "Predecessor(-10)");
    Check(set.Predecessor(-15) == -20, "Predecessor(-15)");

    // PredecessorNRemove
    int predNRem = set.PredecessorNRemove(1); // removes 0
    Check(predNRem == 0 && !set.Exists(0), "PredecessorNRemove(1)");

    // RemovePredecessor
    set.RemovePredecessor(-10); // removes -15
    Check(!set.Exists(-15), "RemovePredecessor(-10)");

    // Successor
    Check(set.Successor(-20) == -10, "Successor(-20)");
    Check(set.Successor(5) == 7, "Successor(5)");
    Check(set.Successor(10) == 15, "Successor(10)");

    // SuccessorNRemove
    int succNRem = set.SuccessorNRemove(5); // removes 7
    Check(succNRem == 7 && !set.Exists(7), "SuccessorNRemove(5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10); // removes -3
    Check(!set.Exists(-3), "RemoveSuccessor(-10)");

    // MinNRemove / MaxNRemove
    int minRemoved = set.MinNRemove(); // -31
    int maxRemoved = set.MaxNRemove(); // 20
    Check(!set.Exists(minRemoved) && !set.Exists(maxRemoved),
          "Check MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax (Set still has many elements)
    set.RemoveMin(); // removes -20
    set.RemoveMax(); // removes 15
    Check(!set.Exists(-20) && !set.Exists(15), "RemoveMin/Max");

    // Resize
    set.Insert(-100);
    set.Insert(200);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<int> vec(4);
    vec[0] = 1; vec[1] = -1; vec[2] = 1; vec[3] = 7;
    lasd::SetLst<int> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetLst<int> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetLst<int> set4(std::move(set3));
    Check(set4.Exists(7) && set4.Exists(1), "Move constructor");

    // Copy assignment
    lasd::SetLst<int> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetLst<int> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1), "Move assignment");

    std::cout << std::endl;
}

void TestSetLstFloat() {
    std::cout << "==== Test SetLst<float> ====" << std::endl;

    lasd::SetLst<float> set;

    // More extensive Insert
    Check(set.Insert(5.5f), "Insert(5.5)");
    Check(set.Insert(-10.1f), "Insert(-10.1)");
    Check(set.Insert(-20.0f), "Insert(-20.0)");
    Check(set.Insert(-3.3f), "Insert(-3.3)");
    Check(set.Insert(0.0f), "Insert(0.0)");
    Check(set.Insert(3.14f), "Insert(3.14)");
    Check(set.Insert(10.0f), "Insert(10.0)");
    Check(set.Insert(15.75f), "Insert(15.75)");
    Check(set.Insert(-30.2f), "Insert(-30.2)");
    Check(set.Insert(8.8f), "Insert(8.8)");
    Check(set.Insert(1.1f), "Insert(1.1)");
    Check(set.Insert(-15.6f), "Insert(-15.6)");
    Check(set.Insert(20.0f), "Insert(20.0)");
    Check(!set.Insert(3.14f), "Insert(3.14) duplicato");

    // Insert rvalue
    float val = 7.7f;
    Check(set.Insert(std::move(val)), "Insert(7.7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30.2f && set[1] == -20.0f, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10.1f), "Exists(-10.1)");
    Check(!set.Exists(42.0f), "Exists(42.0) == false");
    Check(set.Remove(3.14f), "Remove(3.14)");
    Check(!set.Remove(3.14f), "Remove(3.14) already removed");

    // Traverse
    float traverseSum = 0;
    set.Traverse([&](const float& val) {
        traverseSum += val;
    });
    Check(std::abs(traverseSum - (-10.35f)) < 1e-5, "Traverse sum");

    // PreOrderTraverse
    float preSum = 0;
    set.PreOrderTraverse([&](const float& val) {
        preSum += val;
    });
    Check(std::abs(preSum - (-10.35f)) < 1e-5, "PreOrderTraverse sum");

    // PostOrderTraverse
    float postSum = 0;
    set.PostOrderTraverse([&](const float& val) {
        postSum += val;
    });
    Check(std::abs(postSum - (-10.35f)) < 1e-5, "PostOrderTraverse sum");

    // Fold
    float folded = set.Fold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(folded - (-10.35f)) < 1e-5, "Fold sum");

    // PreOrderFold
    float preFolded = set.PreOrderFold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(preFolded - (-10.35f)) < 1e-5, "PreOrderFold sum");

    // PostOrderFold
    float postFolded = set.PostOrderFold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(postFolded - (-10.35f)) < 1e-5, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30.2f, "Min()");
    Check(set.Max() == 20.0f, "Max()");

    // Predecessor
    Check(set.Predecessor(1.1f) == 0.0f, "Predecessor(1.1)");
    Check(set.Predecessor(-10.1f) == -15.6f, "Predecessor(-10.1)");
    Check(set.Predecessor(-15.6f) == -20.0f, "Predecessor(-15.6)");

    // PredecessorNRemove
    float predNRem = set.PredecessorNRemove(1.1f); // removes 0.0
    Check(predNRem == 0.0f && !set.Exists(0.0f), "PredecessorNRemove(1.1)");

    // RemovePredecessor
    set.RemovePredecessor(-10.1f); // removes -15.6
    Check(!set.Exists(-15.6f), "RemovePredecessor(-10.1)");

    // Successor
    Check(set.Successor(-20.0f) == -10.1f, "Successor(-20.0)");
    Check(set.Successor(5.5f) == 7.7f, "Successor(5.5)");
    Check(set.Successor(10.0f) == 15.75f, "Successor(10.0)");

    // SuccessorNRemove
    float succNRem = set.SuccessorNRemove(5.5f); // removes 7.7
    Check(std::abs(succNRem - 7.7f) < 1e-5 && !set.Exists(7.7f), "SuccessorNRemove(5.5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10.1f); // removes -3.3
    Check(!set.Exists(-3.3f), "RemoveSuccessor(-10.1)");

    // MinNRemove / MaxNRemove
    float minRemoved = set.MinNRemove(); // -30.2
    float maxRemoved = set.MaxNRemove(); // 20.0
    Check(!set.Exists(minRemoved) && !set.Exists(maxRemoved),
          "Check MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // removes -20.0
    set.RemoveMax(); // removes 15.75
    Check(!set.Exists(-20.0f) && !set.Exists(15.75f), "RemoveMin/Max");

    // Resize
    set.Insert(-100.0f);
    set.Insert(200.0f);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<float> vec(4);
    vec[0] = 1.0f; vec[1] = -1.0f; vec[2] = 1.0f; vec[3] = 7.0f;
    lasd::SetLst<float> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetLst<float> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetLst<float> set4(std::move(set3));
    Check(set4.Exists(7.0f) && set4.Exists(1.0f), "Move constructor");

    // Copy assignment
    lasd::SetLst<float> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetLst<float> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1.0f), "Move assignment");

    std::cout << std::endl;
}

void TestSetLstDouble() {
    std::cout << "==== Test SetLst<double> ====" << std::endl;

    lasd::SetLst<double> set;

    // More extensive Insert
    Check(set.Insert(5.5), "Insert(5.5)");
    Check(set.Insert(-10.1), "Insert(-10.1)");
    Check(set.Insert(-20.0), "Insert(-20.0)");
    Check(set.Insert(-3.3), "Insert(-3.3)");
    Check(set.Insert(0.0), "Insert(0.0)");
    Check(set.Insert(3.14), "Insert(3.14)");
    Check(set.Insert(10.0), "Insert(10.0)");
    Check(set.Insert(15.75), "Insert(15.75)");
    Check(set.Insert(-30.2), "Insert(-30.2)");
    Check(set.Insert(8.8), "Insert(8.8)");
    Check(set.Insert(1.1), "Insert(1.1)");
    Check(set.Insert(-15.6), "Insert(-15.6)");
    Check(set.Insert(20.0), "Insert(20.0)");
    Check(!set.Insert(3.14), "Insert(3.14) duplicato");

    // Insert rvalue
    double val = 7.7;
    Check(set.Insert(std::move(val)), "Insert(7.7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30.2 && set[1] == -20.0, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10.1), "Exists(-10.1)");
    Check(!set.Exists(42.0), "Exists(42.0) == false");
    Check(set.Remove(3.14), "Remove(3.14)");
    Check(!set.Remove(3.14), "Remove(3.14) already removed");

    // Traverse
    double traverseSum = 0;
    set.Traverse([&](const double& val) {
        traverseSum += val;
    });
    Check(std::abs(traverseSum - (-10.35)) < 1e-10, "Traverse sum");

    // PreOrderTraverse
    double preSum = 0;
    set.PreOrderTraverse([&](const double& val) {
        preSum += val;
    });
    Check(std::abs(preSum - (-10.35)) < 1e-10, "PreOrderTraverse sum");

    // PostOrderTraverse
    double postSum = 0;
    set.PostOrderTraverse([&](const double& val) {
        postSum += val;
    });
    Check(std::abs(postSum - (-10.35)) < 1e-10, "PostOrderTraverse sum");

    // Fold
    double folded = set.Fold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(folded - (-10.35)) < 1e-10, "Fold sum");

    // PreOrderFold
    double preFolded = set.PreOrderFold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(preFolded - (-10.35)) < 1e-10, "PreOrderFold sum");

    // PostOrderFold
    double postFolded = set.PostOrderFold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(postFolded - (-10.35)) < 1e-10, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30.2, "Min()");
    Check(set.Max() == 20.0, "Max()");

    // Predecessor
    Check(set.Predecessor(1.1) == 0.0, "Predecessor(1.1)");
    Check(set.Predecessor(-10.1) == -15.6, "Predecessor(-10.1)");
    Check(set.Predecessor(-15.6) == -20.0, "Predecessor(-15.6)");

    // PredecessorNRemove
    double predNRem = set.PredecessorNRemove(1.1); // removes 0.0
    Check(predNRem == 0.0 && !set.Exists(0.0), "PredecessorNRemove(1.1)");

    // RemovePredecessor
    set.RemovePredecessor(-10.1); // removes -15.6
    Check(!set.Exists(-15.6), "RemovePredecessor(-10.1)");

    // Successor
    Check(set.Successor(-20.0) == -10.1, "Successor(-20.0)");
    Check(set.Successor(5.5) == 7.7, "Successor(5.5)");
    Check(set.Successor(10.0) == 15.75, "Successor(10.0)");

    // SuccessorNRemove
    double succNRem = set.SuccessorNRemove(5.5); // removes 7.7
    Check(std::abs(succNRem - 7.7) < 1e-10 && !set.Exists(7.7), "SuccessorNRemove(5.5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10.1); // removes -3.3
    Check(!set.Exists(-3.3), "RemoveSuccessor(-10.1)");

    // MinNRemove / MaxNRemove
    double minRemoved = set.MinNRemove(); // -30.2
    double maxRemoved = set.MaxNRemove(); // 20.0
    Check(!set.Exists(minRemoved) && !set.Exists(maxRemoved),
          "Check MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // removes -20.0
    set.RemoveMax(); // removes 15.75
    Check(!set.Exists(-20.0) && !set.Exists(15.75), "RemoveMin/Max");

    // Resize
    set.Insert(-100.0);
    set.Insert(200.0);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<double> vec(4);
    vec[0] = 1.0; vec[1] = -1.0; vec[2] = 1.0; vec[3] = 7.0;
    lasd::SetLst<double> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetLst<double> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetLst<double> set4(std::move(set3));
    Check(set4.Exists(7.0) && set4.Exists(1.0), "Move constructor");

    // Copy assignment
    lasd::SetLst<double> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetLst<double> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1.0), "Move assignment");

    std::cout << std::endl;
}

void TestSetLstString() {
    std::cout << "==== Test SetLst<std::string> ====" << std::endl;

    lasd::SetLst<std::string> set;

    // Insert
    Check(set.Insert("banana"), "Insert(\"banana\")");
    Check(set.Insert("apple"), "Insert(\"apple\")");
    Check(set.Insert("cherry"), "Insert(\"cherry\")");
    Check(set.Insert("kiwi"), "Insert(\"kiwi\")");
    Check(set.Insert("grape"), "Insert(\"grape\")");
    Check(set.Insert("lemon"), "Insert(\"lemon\")");
    Check(set.Insert("pear"), "Insert(\"pear\")");
    Check(set.Insert("orange"), "Insert(\"orange\")");
    Check(set.Insert("melon"), "Insert(\"melon\")");
    Check(set.Insert("fig"), "Insert(\"fig\")");
    Check(set.Insert("mango"), "Insert(\"mango\")");
    Check(set.Insert("zebra"), "Insert(\"zebra\")");
    Check(set.Insert("!alert"), "Insert(\"!alert\")");
    Check(!set.Insert("banana"), "Insert(\"banana\") duplicato");

    // Insert rvalue
    std::string str = "lime";
    Check(set.Insert(std::move(str)), "Insert(\"lime\") rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == "!alert" && set[1] == "apple", "operator[] lexicographic order");

    // Exists / Remove
    Check(set.Exists("kiwi"), "Exists(\"kiwi\")");
    Check(!set.Exists("papaya"), "Exists(\"papaya\") == false");
    Check(set.Remove("banana"), "Remove(\"banana\")");
    Check(!set.Remove("banana"), "Remove(\"banana\") already removed");

    // Traverse
    std::string concat;
    set.Traverse([&](const std::string& s) {
        concat += s + "|";
    });
    Check(!concat.empty() && concat.front() == '!', "Traverse concatenation");

    // Fold
    std::string folded = set.Fold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val + "-";
    }, std::string{});
    Check(!folded.empty() && folded.front() == '!', "Fold concatenation");

    // Min / Max
    Check(set.Min() == "!alert", "Min()");
    Check(set.Max() == "zebra", "Max()");

    // Predecessor
    Check(set.Predecessor("fig") == "cherry", "Predecessor(\"fig\")");
    Check(set.Predecessor("lemon") == "kiwi", "Predecessor(\"lemon\")");
    Check(set.Predecessor("mango") == "lime", "Predecessor(\"mango\")");

    // PredecessorNRemove
    std::string p = set.PredecessorNRemove("lemon"); // removes "lime"
    Check(p == "kiwi" && !set.Exists("kiwi"), "PredecessorNRemove(\"lemon\")");

    // RemovePredecessor
    set.RemovePredecessor("melon"); // removes "mango"
    Check(!set.Exists("mango"), "RemovePredecessor(\"melon\")");

    // Successor
    Check(set.Successor("fig") == "grape", "Successor(\"fig\")");
    Check(set.Successor("lemon") == "lime", "Successor(\"lemon\")");
    Check(set.Successor("orange") == "pear", "Successor(\"orange\")");

    // SuccessorNRemove
    std::string s = set.SuccessorNRemove("lemon"); // removes "melon"
    Check(s == "lime" && !set.Exists("lime"), "SuccessorNRemove(\"lemon\")");

    // RemoveSuccessor
    set.RemoveSuccessor("kiwi"); // removes "lemon"
    Check(!set.Exists("lemon"), "RemoveSuccessor(\"kiwi\")");

    // MinNRemove / MaxNRemove
    std::string min = set.MinNRemove(); // "!alert"
    std::string max = set.MaxNRemove(); // "zebra"
    Check(!set.Exists(min) && !set.Exists(max), "MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // "apple"
    set.RemoveMax(); // "pear"
    Check(!set.Exists("apple") && !set.Exists("pear"), "RemoveMin/Max");

    // Resize
    set.Insert("alpha");
    set.Insert("omega");
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<std::string> vec(4);
    vec[0] = "a"; vec[1] = "b"; vec[2] = "a"; vec[3] = "c";
    lasd::SetLst<std::string> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetLst<std::string> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetLst<std::string> set4(std::move(set3));
    Check(set4.Exists("b") && set4.Exists("c"), "Move constructor");

    // Copy assignment
    lasd::SetLst<std::string> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetLst<std::string> set6;
    set6 = std::move(set5);
    Check(set6.Exists("a"), "Move assignment");

    std::cout << std::endl;
}

void TestSetVecInt() {
    std::cout << "==== Test SetVec<int> ====" << std::endl;

    lasd::SetVec<int> set;

    // Insert
    Check(set.Insert(5), "Insert(5)");
    Check(set.Insert(-10), "Insert(-10)");
    Check(set.Insert(-20), "Insert(-20)");
    Check(set.Insert(-3), "Insert(-3)");
    Check(set.Insert(0), "Insert(0)");
    Check(set.Insert(3), "Insert(3)");
    Check(set.Insert(10), "Insert(10)");
    Check(set.Insert(15), "Insert(15)");
    Check(set.Insert(-30), "Insert(-30)");
    Check(set.Insert(8), "Insert(8)");
    Check(set.Insert(1), "Insert(1)");
    Check(set.Insert(-15), "Insert(-15)");
    Check(set.Insert(20), "Insert(20)");
    Check(!set.Insert(3), "Insert(3) duplicato");

    // Insert rvalue
    int r = 7;
    Check(set.Insert(std::move(r)), "Insert(7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30 && set[1] == -20, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10), "Exists(-10)");
    Check(!set.Exists(42), "Exists(42) == false");
    Check(set.Remove(3), "Remove(3)");
    Check(!set.Remove(3), "Remove(3) already removed");

    // Traverse
    int sum = 0;
    set.Traverse([&](const int& val) { sum += val; });
    Check(sum == -12, "Traverse sum");

    // PreOrderTraverse
    int preSum = 0;
    set.PreOrderTraverse([&](const int& val) { preSum += val; });
    Check(preSum == -12, "PreOrderTraverse sum");

    // PostOrderTraverse
    int postSum = 0;
    set.PostOrderTraverse([&](const int& val) { postSum += val; });
    Check(postSum == -12, "PostOrderTraverse sum");

    // Fold
    int folded = set.Fold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(folded == -12, "Fold sum");

    // PreOrderFold
    int preFolded = set.PreOrderFold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(preFolded == -12, "PreOrderFold sum");

    // PostOrderFold
    int postFolded = set.PostOrderFold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(postFolded == -12, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30, "Min()");
    Check(set.Max() == 20, "Max()");

    // Predecessor
    Check(set.Predecessor(1) == 0, "Predecessor(1)");
    Check(set.Predecessor(-10) == -15, "Predecessor(-10)");
    Check(set.Predecessor(-15) == -20, "Predecessor(-15)");

    // PredecessorNRemove
    int pred = set.PredecessorNRemove(1); // removes 0
    Check(pred == 0 && !set.Exists(0), "PredecessorNRemove(1)");

    // RemovePredecessor
    set.RemovePredecessor(-10); // removes -15
    Check(!set.Exists(-15), "RemovePredecessor(-10)");

    // Successor
    Check(set.Successor(-20) == -10, "Successor(-20)");
    Check(set.Successor(5) == 7, "Successor(5)");
    Check(set.Successor(10) == 15, "Successor(10)");

    // SuccessorNRemove
    int succ = set.SuccessorNRemove(5); // removes 7
    Check(succ == 7 && !set.Exists(7), "SuccessorNRemove(5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10); // removes -3
    Check(!set.Exists(-3), "RemoveSuccessor(-10)");

    // MinNRemove / MaxNRemove
    int min = set.MinNRemove(); // -30
    int max = set.MaxNRemove(); // 20
    Check(!set.Exists(min) && !set.Exists(max), "MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // -20
    set.RemoveMax(); // 15
    Check(!set.Exists(-20) && !set.Exists(15), "RemoveMin/Max");

    // Resize
    set.Clear();
    set.Insert(-100);
    set.Insert(200);
    set.Insert(0);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<int> vec(4);
    vec[0] = 1; vec[1] = -1; vec[2] = 1; vec[3] = 7;
    lasd::SetVec<int> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetVec<int> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetVec<int> set4(std::move(set3));
    Check(set4.Exists(7) && set4.Exists(1), "Move constructor");

    // Copy assignment
    lasd::SetVec<int> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetVec<int> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1), "Move assignment");

    std::cout << std::endl;
}

void TestSetVecFloat() {
    std::cout << "==== Test SetVec<float> ====" << std::endl;

    lasd::SetVec<float> set;

    // Insert
    Check(set.Insert(5.5f), "Insert(5.5)");
    Check(set.Insert(-10.1f), "Insert(-10.1)");
    Check(set.Insert(-20.0f), "Insert(-20.0)");
    Check(set.Insert(-3.3f), "Insert(-3.3)");
    Check(set.Insert(0.0f), "Insert(0.0)");
    Check(set.Insert(3.14f), "Insert(3.14)");
    Check(set.Insert(10.0f), "Insert(10.0)");
    Check(set.Insert(15.75f), "Insert(15.75)");
    Check(set.Insert(-30.2f), "Insert(-30.2)");
    Check(set.Insert(8.8f), "Insert(8.8)");
    Check(set.Insert(1.1f), "Insert(1.1)");
    Check(set.Insert(-15.6f), "Insert(-15.6)");
    Check(set.Insert(20.0f), "Insert(20.0)");
    Check(!set.Insert(3.14f), "Insert(3.14) duplicato");

    // Insert rvalue
    float r = 7.7f;
    Check(set.Insert(std::move(r)), "Insert(7.7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30.2f && set[1] == -20.0f, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10.1f), "Exists(-10.1)");
    Check(!set.Exists(99.99f), "Exists(99.99) == false");
    Check(set.Remove(3.14f), "Remove(3.14)");
    Check(!set.Remove(3.14f), "Remove(3.14) already removed");

    // Traverse
    float sum = 0;
    set.Traverse([&](const float& val) { sum += val; });
    Check(std::abs(sum - (-10.35f)) < 1e-2, "Traverse sum");

    // PreOrderTraverse
    float preSum = 0;
    set.PreOrderTraverse([&](const float& val) { preSum += val; });
    Check(std::abs(preSum - (-10.35f)) < 1e-2, "PreOrderTraverse sum");

    // PostOrderTraverse
    float postSum = 0;
    set.PostOrderTraverse([&](const float& val) { postSum += val; });
    Check(std::abs(postSum - (-10.35f)) < 1e-2, "PostOrderTraverse sum");

    // Fold
    float folded = set.Fold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(folded - (-10.35f)) < 1e-2, "Fold sum");

    // PreOrderFold
    float preFolded = set.PreOrderFold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(preFolded - (-10.35f)) < 1e-2, "PreOrderFold sum");

    // PostOrderFold
    float postFolded = set.PostOrderFold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(postFolded - (-10.35f)) < 1e-2, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30.2f, "Min()");
    Check(set.Max() == 20.0f, "Max()");

    // Predecessor
    Check(set.Predecessor(1.1f) == 0.0f, "Predecessor(1.1)");
    Check(set.Predecessor(-10.1f) == -15.6f, "Predecessor(-10.1)");
    Check(set.Predecessor(-15.6f) == -20.0f, "Predecessor(-15.6)");

    // PredecessorNRemove
    float pred = set.PredecessorNRemove(1.1f); // removes 0.0
    Check(pred == 0.0f && !set.Exists(0.0f), "PredecessorNRemove(1.1)");

    // RemovePredecessor
    set.RemovePredecessor(-10.1f); // removes -15.6
    Check(!set.Exists(-15.6f), "RemovePredecessor(-10.1)");

    // Successor
    Check(set.Successor(-20.0f) == -10.1f, "Successor(-20.0)");
    Check(set.Successor(5.5f) == 7.7f, "Successor(5.5)");
    Check(set.Successor(10.0f) == 15.75f, "Successor(10.0)");

    // SuccessorNRemove
    float succ = set.SuccessorNRemove(5.5f); // removes 7.7
    Check(succ == 7.7f && !set.Exists(7.7f), "SuccessorNRemove(5.5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10.1f); // removes -3.3
    Check(!set.Exists(-3.3f), "RemoveSuccessor(-10.1)");

    // MinNRemove / MaxNRemove
    float min = set.MinNRemove(); // -30.2
    float max = set.MaxNRemove(); // 20.0
    Check(!set.Exists(min) && !set.Exists(max), "MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // -20.0
    set.RemoveMax(); // 15.75
    Check(!set.Exists(-20.0f) && !set.Exists(15.75f), "RemoveMin/Max");

    // Resize
    set.Clear();
    set.Insert(-100.0f);
    set.Insert(200.0f);
    set.Insert(0.0f);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<float> vec(4);
    vec[0] = 1.0f; vec[1] = -1.0f; vec[2] = 1.0f; vec[3] = 7.0f;
    lasd::SetVec<float> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetVec<float> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetVec<float> set4(std::move(set3));
    Check(set4.Exists(7.0f) && set4.Exists(1.0f), "Move constructor");

    // Copy assignment
    lasd::SetVec<float> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetVec<float> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1.0f), "Move assignment");

    std::cout << std::endl;
}

void TestSetVecDouble() {
    std::cout << "==== Test SetVec<double> ====" << std::endl;

    lasd::SetVec<double> set;

    // Insert
    Check(set.Insert(5.5), "Insert(5.5)");
    Check(set.Insert(-10.1), "Insert(-10.1)");
    Check(set.Insert(-20.0), "Insert(-20.0)");
    Check(set.Insert(-3.3), "Insert(-3.3)");
    Check(set.Insert(0.0), "Insert(0.0)");
    Check(set.Insert(3.14), "Insert(3.14)");
    Check(set.Insert(10.0), "Insert(10.0)");
    Check(set.Insert(15.75), "Insert(15.75)");
    Check(set.Insert(-30.2), "Insert(-30.2)");
    Check(set.Insert(8.8), "Insert(8.8)");
    Check(set.Insert(1.1), "Insert(1.1)");
    Check(set.Insert(-15.6), "Insert(-15.6)");
    Check(set.Insert(20.0), "Insert(20.0)");
    Check(!set.Insert(3.14), "Insert(3.14) duplicato");

    // Insert rvalue
    double r = 7.7;
    Check(set.Insert(std::move(r)), "Insert(7.7) rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == -30.2 && set[1] == -20.0, "operator[] ascending order");

    // Exists / Remove
    Check(set.Exists(-10.1), "Exists(-10.1)");
    Check(!set.Exists(99.99), "Exists(99.99) == false");
    Check(set.Remove(3.14), "Remove(3.14)");
    Check(!set.Remove(3.14), "Remove(3.14) already removed");

    // Traverse
    double sum = 0;
    set.Traverse([&](const double& val) { sum += val; });
    Check(std::abs(sum - (-10.35)) < 1e-9, "Traverse sum");

    // PreOrderTraverse
    double preSum = 0;
    set.PreOrderTraverse([&](const double& val) { preSum += val; });
    Check(std::abs(preSum - (-10.35)) < 1e-9, "PreOrderTraverse sum");

    // PostOrderTraverse
    double postSum = 0;
    set.PostOrderTraverse([&](const double& val) { postSum += val; });
    Check(std::abs(postSum - (-10.35)) < 1e-9, "PostOrderTraverse sum");

    // Fold
    double folded = set.Fold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(folded - (-10.35)) < 1e-9, "Fold sum");

    // PreOrderFold
    double preFolded = set.PreOrderFold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(preFolded - (-10.35)) < 1e-9, "PreOrderFold sum");

    // PostOrderFold
    double postFolded = set.PostOrderFold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(postFolded - (-10.35)) < 1e-9, "PostOrderFold sum");

    // Min / Max
    Check(set.Min() == -30.2, "Min()");
    Check(set.Max() == 20.0, "Max()");

    // Predecessor
    Check(set.Predecessor(1.1) == 0.0, "Predecessor(1.1)");
    Check(set.Predecessor(-10.1) == -15.6, "Predecessor(-10.1)");
    Check(set.Predecessor(-15.6) == -20.0, "Predecessor(-15.6)");

    // PredecessorNRemove
    double pred = set.PredecessorNRemove(1.1); // removes 0.0
    Check(pred == 0.0 && !set.Exists(0.0), "PredecessorNRemove(1.1)");

    // RemovePredecessor
    set.RemovePredecessor(-10.1); // removes -15.6
    Check(!set.Exists(-15.6), "RemovePredecessor(-10.1)");

    // Successor
    Check(set.Successor(-20.0) == -10.1, "Successor(-20.0)");
    Check(set.Successor(5.5) == 7.7, "Successor(5.5)");
    Check(set.Successor(10.0) == 15.75, "Successor(10.0)");

    // SuccessorNRemove
    double succ = set.SuccessorNRemove(5.5); // removes 7.7
    Check(succ == 7.7 && !set.Exists(7.7), "SuccessorNRemove(5.5)");

    // RemoveSuccessor
    set.RemoveSuccessor(-10.1); // removes -3.3
    Check(!set.Exists(-3.3), "RemoveSuccessor(-10.1)");

    // MinNRemove / MaxNRemove
    double min = set.MinNRemove(); // -30.2
    double max = set.MaxNRemove(); // 20.0
    Check(!set.Exists(min) && !set.Exists(max), "MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // -20.0
    set.RemoveMax(); // 15.75
    Check(!set.Exists(-20.0) && !set.Exists(15.75), "RemoveMin/Max");

    // Resize
    set.Clear();
    set.Insert(-100.0);
    set.Insert(200.0);
    set.Insert(0.0);
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<double> vec(4);
    vec[0] = 1.0; vec[1] = -1.0; vec[2] = 1.0; vec[3] = 7.0;
    lasd::SetVec<double> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetVec<double> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetVec<double> set4(std::move(set3));
    Check(set4.Exists(7.0) && set4.Exists(1.0), "Move constructor");

    // Copy assignment
    lasd::SetVec<double> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetVec<double> set6;
    set6 = std::move(set5);
    Check(set6.Exists(-1.0), "Move assignment");

    std::cout << std::endl;
}

void TestSetVecString() {
    std::cout << "==== Test SetVec<std::string> ====" << std::endl;

    lasd::SetVec<std::string> set;

    // Insert
    Check(set.Insert("banana"), "Insert(\"banana\")");
    Check(set.Insert("apple"), "Insert(\"apple\")");
    Check(set.Insert("cherry"), "Insert(\"cherry\")");
    Check(set.Insert("kiwi"), "Insert(\"kiwi\")");
    Check(set.Insert("grape"), "Insert(\"grape\")");
    Check(set.Insert("lemon"), "Insert(\"lemon\")");
    Check(set.Insert("pear"), "Insert(\"pear\")");
    Check(set.Insert("orange"), "Insert(\"orange\")");
    Check(set.Insert("melon"), "Insert(\"melon\")");
    Check(set.Insert("fig"), "Insert(\"fig\")");
    Check(set.Insert("mango"), "Insert(\"mango\")");
    Check(set.Insert("zebra"), "Insert(\"zebra\")");
    Check(set.Insert("!alert"), "Insert(\"!alert\")");
    Check(!set.Insert("banana"), "Insert(\"banana\") duplicato");

    // Insert rvalue
    std::string rv = "lime";
    Check(set.Insert(std::move(rv)), "Insert(\"lime\") rvalue");

    // Size / Empty
    Check(set.Size() == 14, "Size == 14");
    Check(!set.Empty(), "Empty == false");

    // Access
    Check(set[0] == "!alert" && set[1] == "apple", "operator[] lexicographic order");

    // Exists / Remove
    Check(set.Exists("grape"), "Exists(\"grape\")");
    Check(!set.Exists("coconut"), "Exists(\"coconut\") == false");
    Check(set.Remove("cherry"), "Remove(\"cherry\")");
    Check(!set.Remove("cherry"), "Remove(\"cherry\") already removed");

    // Traverse: concatenation
    std::string concat;
    set.Traverse([&](const std::string& val) {
        concat += val + "|";
    });
    Check(!concat.empty() && concat.find("banana") != std::string::npos, "Traverse concatenation");

    // PreOrderTraverse
    std::string preConcat;
    set.PreOrderTraverse([&](const std::string& val) {
        preConcat += val + ":";
    });
    Check(!preConcat.empty() && preConcat.find("grape") != std::string::npos, "PreOrderTraverse concatenation");

    // PostOrderTraverse
    std::string postConcat;
    set.PostOrderTraverse([&](const std::string& val) {
        postConcat += val + "-";
    });
    Check(postConcat.find("apple") != std::string::npos, "PostOrderTraverse concatenation");

    // Fold
    std::string folded = set.Fold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, "");
    Check(!folded.empty() && folded.find("apple") != std::string::npos, "Fold concatenation");

    // PreOrderFold
    std::string preFolded = set.PreOrderFold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, "");
    Check(preFolded.length() == folded.length(), "PreOrderFold concatenation");

    // PostOrderFold
    std::string postFolded = set.PostOrderFold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, "");
    Check(postFolded.length() == folded.length(), "PostOrderFold concatenation");

    // Min / Max
    Check(set.Min() == "!alert", "Min()");
    Check(set.Max() == "zebra", "Max()");

    // Predecessor
    Check(set.Predecessor("fig") == "banana", "Predecessor(\"fig\")");
    Check(set.Predecessor("lemon") == "kiwi", "Predecessor(\"lemon\")");
    Check(set.Predecessor("mango") == "lime", "Predecessor(\"mango\")");

    // PredecessorNRemove
    std::string p = set.PredecessorNRemove("melon");
    Check(p == "mango" && !set.Exists("mango"), "PredecessorNRemove(\"melon\")");

    // RemovePredecessor
    set.RemovePredecessor("orange"); // removes "melon"
    Check(!set.Exists("melon"), "RemovePredecessor(\"orange\")");

    // Successor
    Check(set.Successor("fig") == "grape", "Successor(\"fig\")");
    Check(set.Successor("kiwi") == "lemon", "Successor(\"kiwi\")");
    Check(set.Successor("pear") == "zebra", "Successor(\"pear\")");

    // SuccessorNRemove
    std::string s = set.SuccessorNRemove("kiwi"); // removes "lemon"
    Check(s == "lemon" && !set.Exists("lemon"), "SuccessorNRemove(\"kiwi\")");

    // RemoveSuccessor
    set.RemoveSuccessor("fig"); // removes "grape"
    Check(!set.Exists("grape"), "RemoveSuccessor(\"fig\")");

    // MinNRemove / MaxNRemove
    std::string min = set.MinNRemove(); // "!alert"
    std::string max = set.MaxNRemove(); // "zebra"
    Check(!set.Exists(min) && !set.Exists(max), "MinNRemove/MaxNRemove");

    // RemoveMin / RemoveMax
    set.RemoveMin(); // apple
    set.RemoveMax(); // pear
    Check(!set.Exists("apple") && !set.Exists("pear"), "RemoveMin/Max");

    // Resize
    set.Clear();
    set.Insert("alpha");
    set.Insert("omega");
    set.Insert("beta");
    set.Resize(3);
    Check(set.Size() == 3, "Resize(3)");

    // Clear
    set.Clear();
    Check(set.Empty(), "Clear() empties");

    // Constructor from container
    lasd::Vector<std::string> vec(4);
    vec[0] = "x"; vec[1] = "y"; vec[2] = "x"; vec[3] = "z";
    lasd::SetVec<std::string> set2(vec);
    Check(set2.Size() == 3, "Constructor from container with duplicate");

    // Copy constructor
    lasd::SetVec<std::string> set3(set2);
    Check(set3 == set2, "Copy constructor");

    // Move constructor
    lasd::SetVec<std::string> set4(std::move(set3));
    Check(set4.Exists("x") && set4.Exists("z"), "Move constructor");

    // Copy assignment
    lasd::SetVec<std::string> set5;
    set5 = set4;
    Check(set5 == set4, "Copy assignment");

    // Move assignment
    lasd::SetVec<std::string> set6;
    set6 = std::move(set5);
    Check(set6.Exists("y"), "Move assignment");

    std::cout << std::endl;
}

void RunAllTests1() {

    TestResetCounter();

    TestFullContainer();
    TestFullDictionaryInt();
    TestFullLinearContainerInt();

    TestVectorInt();
    TestVectorFloat();
    TestVectorDouble();
    TestVectorString();

    TestListInt();
    TestListFloat();
    TestListDouble();
    TestListString();

    TestSetLstInt();
    TestSetLstFloat();
    TestSetLstDouble();
    TestSetLstString();
    
    TestSetVecInt();
    TestSetVecFloat();
    TestSetVecDouble();
    TestSetVecString();

    std::cout << "== TOTAL RESULTS ES1 ==" << std::endl;
    std::cout << "Total tests executed: " << totalTests << std::endl;
    std::cout << "Failed tests:         " << failedTests << std::endl;
    std::cout << std::endl;
}

void TestHeapVecInt() {
    std::cout << "==== Test HeapVec<int> ====" << std::endl;

    // Construction from TraversableContainer (Vector)
    lasd::Vector<int> vec;
    vec.Resize(5);
    vec[0] = 10; vec[1] = 5; vec[2] = 20; vec[3] = 15; vec[4] = 2;

    lasd::HeapVec<int> heap(vec);
    Check(heap.Size() == 5, "Constructor from TraversableContainer");
    Check(heap.IsHeap(), "IsHeap() == true after Heapify in the constructor");

    // Manual Heapify on a non-heap Vector
    lasd::Vector<int> unordered;
    unordered.Resize(4);
    unordered[0] = 1; unordered[1] = 8; unordered[2] = 3; unordered[3] = 7;
    lasd::HeapVec<int> heap2(unordered);
    Check(heap2.IsHeap(), "Heapify() arranges the correct structure");

    // Sort (heap sort in place)
    heap.Sort(); // after sort, it is no longer a heap
    Check(heap[0] == 2 && heap[4] == 20, "Sort() orders correctly");
    Check(!heap.IsHeap(), "After Sort() it is no longer a heap");

    // Traverse
    int sum = 0;
    heap.Traverse([&](const int& val) { sum += val; });
    Check(sum == 52, "Traverse sum of values == 52");

    // PreOrderTraverse
    int sumPre = 0;
    heap.PreOrderTraverse([&](const int& val) { sumPre += val; });
    Check(sumPre == 52, "PreOrderTraverse sum of values");

    // PostOrderTraverse
    int prodPost = 1;
    heap.PostOrderTraverse([&](const int& val) { prodPost *= val; });
    Check(prodPost == 2 * 5 * 10 * 15 * 20, "PostOrderTraverse correct product");

    // Fold
    int foldSum = heap.Fold<int>([](const int& val, const int& acc) { return acc + val; }, 0);
    Check(foldSum == 52, "Fold sum");

    int foldProd = heap.PostOrderFold<int>([](const int& val, const int& acc) { return acc * val; }, 1);
    Check(foldProd == prodPost, "PostOrderFold product");

    // Copy
    lasd::HeapVec<int> copy(heap);
    Check(copy == heap, "Copy constructor e operator==");

    // Move
    lasd::HeapVec<int> moved(std::move(copy));
    Check(moved.Size() == 5 && copy.Size() == 0, "Move constructor");

    // Copy assignment
    lasd::HeapVec<int> assign;
    assign = heap;
    Check(assign == heap, "Copy assignment");

    // Move assignment
    lasd::HeapVec<int> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 5 && assign.Size() == 0, "Move assignment");

    // Resize and Clear
    movedAssign.Resize(10);
    Check(movedAssign.Size() == 10, "Resize(10)");

    movedAssign.Resize(3);
    Check(movedAssign.Size() == 3, "Resize(3)");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() final");

    std::cout << std::endl;
}

void TestHeapVecFloat() {
    std::cout << "==== Test HeapVec<float> ====" << std::endl;

    // Construction from TraversableContainer
    lasd::Vector<float> vec;
    vec.Resize(5);
    vec[0] = 1.5f; vec[1] = 3.2f; vec[2] = -2.0f; vec[3] = 7.4f; vec[4] = 0.5f;

    lasd::HeapVec<float> heap(vec);
    Check(heap.Size() == 5, "Constructor from TraversableContainer<float>");
    Check(heap.IsHeap(), "IsHeap() == true after Heapify");

    // Sort
    heap.Sort();
    Check(heap[0] <= heap[1] && heap[1] <= heap[2], "Sort() orders the floats correctly");
    Check(!heap.IsHeap(), "After Sort() it is no longer a heap");

    // Traverse
    float sum = 0.0f;
    heap.Traverse([&](const float& val) { sum += val; });
    Check(std::abs(sum - 10.6f) < 0.001f, "Traverse sum float");

    // Fold
    float foldSum = heap.Fold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(foldSum - 10.6f) < 0.001f, "Fold sum float ≈ Traverse");

    float preSum = heap.PreOrderFold<float>(
    [](const float& val, const float& acc) {
        return acc + val;
    },
    0.0f
    );
    Check(std::abs(preSum - 10.6f) < 0.001f, "PreOrderFold sum float");
    
    // PostOrderFold: product
    float postProd = heap.PostOrderFold<float>(
        [](const float& val, const float& acc) { return acc * val; },
        1.0f
    );
    Check(std::abs(postProd - (1.5f * 3.2f * -2.0f * 7.4f * 0.5f)) < 0.001f, "PostOrderFold product float");

    // Traverse orders
    float sumPre = 0, sumPost = 0;
    heap.PreOrderTraverse([&](const float& val) { sumPre += val; });
    heap.PostOrderTraverse([&](const float& val) { sumPost += val; });
    Check(std::abs(sumPre - sum) < 0.01f, "PreOrderTraverse sum ≈ Traverse");
    Check(std::abs(sumPost - sum) < 0.01f, "PostOrderTraverse sum ≈ Traverse");

    // Copy and move
    lasd::HeapVec<float> copy(heap);
    Check(copy.Size() == heap.Size(), "Copy constructor float (size check)");

    lasd::HeapVec<float> moved(std::move(copy));
    Check(moved.Size() == 5 && copy.Size() == 0, "Move constructor float");

    lasd::HeapVec<float> assign;
    assign = heap;
    Check(assign.Size() == heap.Size(), "Copy assignment float (size check)");

    lasd::HeapVec<float> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 5 && assign.Size() == 0, "Move assignment float");

    // Resize and Clear
    movedAssign.Resize(10);
    Check(movedAssign.Size() == 10, "Resize(10) float");

    movedAssign.Resize(3);
    Check(movedAssign.Size() == 3, "Resize(3) float");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() float");

    std::cout << std::endl;
}

void TestHeapVecDouble() {
    std::cout << "==== Test HeapVec<double> ====" << std::endl;

    // Creation of Vector<double>
    lasd::Vector<double> vec;
    vec.Resize(5);
    vec[0] = 2.1; vec[1] = 4.4; vec[2] = -1.2; vec[3] = 5.7; vec[4] = 0.6;

    // Heap from TraversableContainer
    lasd::HeapVec<double> heap(vec);
    Check(heap.Size() == 5, "Constructor from TraversableContainer<double>");
    Check(heap.IsHeap(), "IsHeap() == true after Heapify");

    // Sort
    heap.Sort();
    Check(heap[0] <= heap[1] && heap[1] <= heap[2], "Sort() orders the doubles correctly");
    Check(!heap.IsHeap(), "After Sort() it is no longer a heap");

    // Traverse
    double sum = 0.0;
    heap.Traverse([&](const double& val) { sum += val; });
    Check(std::abs(sum - 11.6) < 0.001, "Traverse sum double == 11.6");

    // Fold sum
    double foldSum = heap.Fold<double>(
        [](const double& val, const double& acc) { return acc + val; }, 0.0);
    Check(std::abs(foldSum - 11.6) < 0.001, "Fold sum double == 11.6");

    // PreOrderFold sum
    double preSum = heap.PreOrderFold<double>(
        [](const double& val, const double& acc) { return acc + val; }, 0.0);
    Check(std::abs(preSum - 11.6) < 0.001, "PreOrderFold sum double == 11.6");

    // PostOrderFold product
    double postProd = heap.PostOrderFold<double>(
        [](const double& val, const double& acc) { return acc * val; }, 1.0);
    Check(std::abs(postProd - (2.1 * 4.4 * -1.2 * 5.7 * 0.6)) < 0.001, "PostOrderFold correct product double");

    // Traverse orders
    double sumPre = 0, sumPost = 0;
    heap.PreOrderTraverse([&](const double& val) { sumPre += val; });
    heap.PostOrderTraverse([&](const double& val) { sumPost += val; });
    Check(std::abs(sumPre - 11.6) < 0.001, "PreOrderTraverse sum double == 11.6");
    Check(std::abs(sumPost - 11.6) < 0.001, "PostOrderTraverse sum double == 11.6");

    // Copy and move
    lasd::HeapVec<double> copy(heap);
    Check(copy == heap, "Copy constructor double");

    lasd::HeapVec<double> moved(std::move(copy));
    Check(moved.Size() == 5 && copy.Size() == 0, "Move constructor double");

    lasd::HeapVec<double> assign;
    assign = heap;
    Check(assign == heap, "Copy assignment double");

    lasd::HeapVec<double> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 5 && assign.Size() == 0, "Move assignment double");

    // Resize and Clear
    movedAssign.Resize(10);
    Check(movedAssign.Size() == 10, "Resize(10) double");

    movedAssign.Resize(3);
    Check(movedAssign.Size() == 3, "Resize(3) double");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() double");

    std::cout << std::endl;
}

void TestHeapVecString() {
    std::cout << "==== Test HeapVec<string> ====" << std::endl;

    // Vector of strings
    lasd::Vector<std::string> vec;
    vec.Resize(5);
    vec[0] = "alpha";
    vec[1] = "zeta";
    vec[2] = "beta";
    vec[3] = "delta";
    vec[4] = "gamma";

    // HeapVec from TraversableContainer
    lasd::HeapVec<std::string> heap(vec);
    Check(heap.Size() == 5, "Constructor from TraversableContainer<string>");
    Check(heap.IsHeap(), "IsHeap() on string after Heapify");

    // Traverse: concatenation
    std::string concat;
    heap.Traverse([&](const std::string& s) { concat += s + " "; });
    Check(concat.length() > 0, "Traverse concat string not empty");

    // Fold: concatenation
    std::string folded = heap.Fold<std::string>(
        [](const std::string& val, const std::string& acc) {
            return acc + val + "-";
        },
        ""
    );
    Check(!folded.empty(), "Fold concat string not empty");

    // PreOrderFold: concat
    std::string pre = heap.PreOrderFold<std::string>(
        [](const std::string& val, const std::string& acc) {
            return acc + val;
        },
        ""
    );
    Check(!pre.empty(), "PreOrderFold string not empty");

    // PostOrderFold: concat
    std::string post = heap.PostOrderFold<std::string>(
        [](const std::string& val, const std::string& acc) {
            return acc + val;
        },
        ""
    );
    Check(!post.empty(), "PostOrderFold string not empty");

    // Pre/Post Traverse
    std::string preTrav, postTrav;
    heap.PreOrderTraverse([&](const std::string& val) { preTrav += val; });
    heap.PostOrderTraverse([&](const std::string& val) { postTrav += val; });
    Check(!preTrav.empty(), "PreOrderTraverse string not empty");
    Check(!postTrav.empty(), "PostOrderTraverse string not empty");

    // Sort and verification
    heap.Sort();
    Check(heap[0] <= heap[1] && heap[1] <= heap[2], "Sort() on string");

    // Copy
    lasd::HeapVec<std::string> copy(heap);
    Check(copy == heap, "Copy constructor string");

    // Move
    lasd::HeapVec<std::string> moved(std::move(copy));
    Check(moved.Size() == 5 && copy.Size() == 0, "Move constructor string");

    // Copy assignment
    lasd::HeapVec<std::string> assign;
    assign = heap;
    Check(assign == heap, "Copy assignment string");

    // Move assignment
    lasd::HeapVec<std::string> movedAssign;
    movedAssign = std::move(assign);
    Check(movedAssign.Size() == 5 && assign.Size() == 0, "Move assignment string");

    // Resize and Clear
    movedAssign.Resize(10);
    Check(movedAssign.Size() == 10, "Resize(10) string");

    movedAssign.Resize(3);
    Check(movedAssign.Size() == 3, "Resize(3) string");

    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() string");

    std::cout << std::endl;
}

void TestHeapVecSortEdgeCases() {
    std::cout << "==== Test HeapVec::Sort() edge cases ====" << std::endl;

    // Regression: Sort() on an empty heap used to underflow (size - 1 with
    // size == 0) and throw out_of_range inside a noexcept method -> terminate.
    lasd::HeapVec<int> empty;
    empty.Sort();
    Check(empty.Size() == 0, "Sort() on empty heap does not crash");
    Check(empty.Empty(), "Empty heap is still empty after Sort()");

    // Single element: nothing to do, must stay intact.
    lasd::Vector<int> one;
    one.Resize(1);
    one[0] = 42;
    lasd::HeapVec<int> single(one);
    single.Sort();
    Check(single.Size() == 1 && single[0] == 42, "Sort() on single element keeps it");

    // Two elements out of order.
    lasd::Vector<int> two;
    two.Resize(2);
    two[0] = 2; two[1] = 1;
    lasd::HeapVec<int> pair(two);
    pair.Sort();
    Check(pair[0] == 1 && pair[1] == 2, "Sort() on two elements orders ascending");

    // Already sorted input stays sorted.
    lasd::Vector<int> asc;
    asc.Resize(5);
    asc[0] = 1; asc[1] = 2; asc[2] = 3; asc[3] = 4; asc[4] = 5;
    lasd::HeapVec<int> sorted(asc);
    sorted.Sort();
    Check(sorted[0] == 1 && sorted[4] == 5, "Sort() on already-sorted input");

    // All-equal elements.
    lasd::Vector<int> eq;
    eq.Resize(4);
    eq[0] = 7; eq[1] = 7; eq[2] = 7; eq[3] = 7;
    lasd::HeapVec<int> equal(eq);
    equal.Sort();
    Check(equal[0] == 7 && equal[3] == 7, "Sort() on all-equal elements");

    // Duplicates: result must be non-decreasing.
    lasd::Vector<int> dup;
    dup.Resize(6);
    dup[0] = 3; dup[1] = 1; dup[2] = 3; dup[3] = 1; dup[4] = 2; dup[5] = 3;
    lasd::HeapVec<int> duplicates(dup);
    duplicates.Sort();
    bool nonDecreasing = true;
    for (ulong i = 1; i < duplicates.Size(); ++i)
        if (duplicates[i - 1] > duplicates[i]) nonDecreasing = false;
    Check(nonDecreasing, "Sort() with duplicates is non-decreasing");

    std::cout << std::endl;
}

void TestPQHeapInt() {
    std::cout << "==== Test PQHeap<int> ====" << std::endl;

    lasd::PQHeap<int> pq;

    // Insert (copy)
    pq.Insert(30);
    pq.Insert(10);
    pq.Insert(50);
    pq.Insert(20);
    pq.Insert(40);

    Check(pq.Size() == 5, "Insert copy: Size == 5");
    Check(pq.Tip() == 50, "Tip() == 50");
    Check(!pq.Empty(), "Empty() == false");

    // Insert (move)
    int x = 60;
    pq.Insert(std::move(x));
    Check(pq.Tip() == 60, "Insert move: Tip() == 60");

    // TipNRemove
    int max = pq.TipNRemove();
    Check(max == 60, "TipNRemove() == 60");
    Check(pq.Tip() == 50, "Tip() == 50 after TipNRemove");

    // RemoveTip
    pq.RemoveTip();
    Check(pq.Tip() == 40, "RemoveTip: Tip() == 40");

    // Change (copy)
    pq.Change(1, 100); // changes the second element
    Check(pq.Tip() == 100, "Change(i, value): Tip() == 100");

    // Change (move)
    int newVal = 25;
    pq.Change(2, std::move(newVal));
    Check(pq.Exists(25), "Change(i, move) inserted 25");
    Check(pq.Tip() == 100, "Change move did not change the Tip if not necessary");

    // Fold: sum
    int sum = pq.Fold<int>([](const int& val, const int& acc) {
        return acc + val;
    }, 0);
    Check(sum >= 100, "Fold<int> sum >= 100");

    // Traverse: sum
    int traverseSum = 0;
    pq.Traverse([&](const int& val) { traverseSum += val; });
    Check(traverseSum == sum, "Traverse sum == Fold");

    // PreOrderFold
    int preSum = pq.PreOrderFold<int>(
        [](const int& val, const int& acc) { return acc + val; }, 0);
    Check(preSum == sum, "PreOrderFold sum == Fold");

    // PostOrderFold
    int postSum = pq.PostOrderFold<int>(
        [](const int& val, const int& acc) { return acc + val; }, 0);
    Check(postSum == sum, "PostOrderFold sum == Fold");

    // Pre/PostOrderTraverse
    int preTraverseSum = 0, postTraverseSum = 0;
    pq.PreOrderTraverse([&](const int& val) { preTraverseSum += val; });
    pq.PostOrderTraverse([&](const int& val) { postTraverseSum += val; });
    Check(preTraverseSum == sum, "PreOrderTraverse sum == Fold");
    Check(postTraverseSum == sum, "PostOrderTraverse sum == Fold");

    // Copy constructor
    lasd::PQHeap<int> copy(pq);
    Check(copy.Size() == pq.Size(), "Copy constructor (size check)");

    // Move constructor
    lasd::PQHeap<int> moved(std::move(copy));
    Check(moved.Size() == pq.Size() && copy.Empty(), "Move constructor");

    // Copy assignment
    lasd::PQHeap<int> assigned;
    assigned = pq;
    Check(assigned.Size() == pq.Size(), "Copy assignment (size check)");

    // Move assignment
    lasd::PQHeap<int> movedAssign;
    movedAssign = std::move(assigned);
    Check(movedAssign.Size() == pq.Size() && assigned.Empty(), "Move assignment");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() empties the PQ");

    // Exceptions
    bool thrown = false;
    try {
        movedAssign.Tip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "Tip() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.TipNRemove();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "TipNRemove() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.RemoveTip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "RemoveTip() on empty PQ throws exception");

    thrown = false;
    try {
        pq.Change(99, 0);
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    Check(thrown, "Change(i out-of-range) throws exception");

    std::cout << std::endl;
}

void TestPQHeapFloat() {
    std::cout << "==== Test PQHeap<float> ====" << std::endl;

    lasd::PQHeap<float> pq;

    // Insert (copy)
    pq.Insert(3.5f);
    pq.Insert(1.2f);
    pq.Insert(4.8f);
    pq.Insert(2.0f);
    pq.Insert(0.9f);

    Check(pq.Size() == 5, "Insert copy: Size == 5");
    Check(std::abs(pq.Tip() - 4.8f) < 0.001f, "Tip() == 4.8");
    Check(!pq.Empty(), "Empty() == false");

    // Insert (move)
    float x = 7.1f;
    pq.Insert(std::move(x));
    Check(std::abs(pq.Tip() - 7.1f) < 0.001f, "Insert move: Tip() == 7.1");

    // TipNRemove
    float max = pq.TipNRemove();
    Check(std::abs(max - 7.1f) < 0.001f, "TipNRemove() == 7.1");
    Check(std::abs(pq.Tip() - 4.8f) < 0.001f, "Tip() == 4.8 after TipNRemove");

    // RemoveTip
    pq.RemoveTip();
    Check(std::abs(pq.Tip() - 3.5f) < 0.001f, "RemoveTip: Tip() == 3.5");

    // Change (copy)
    pq.Change(1, 10.0f);
    Check(std::abs(pq.Tip() - 10.0f) < 0.001f, "Change(i, val): Tip() == 10.0");

    // Change (move)
    float y = 1.5f;
    pq.Change(2, std::move(y));
    Check(pq.Exists(1.5f), "Change(i, move) inserted 1.5");
    Check(std::abs(pq.Tip() - 10.0f) < 0.001f, "Change move: Tip remains 10.0");

    // Fold: sum
    float foldSum = pq.Fold<float>([](const float& val, const float& acc) {
        return acc + val;
    }, 0.0f);
    Check(std::abs(foldSum - 16.2f) < 0.01f, "Fold<float> sum");

    // Traverse
    float traverseSum = 0.0f;
    pq.Traverse([&](const float& val) { traverseSum += val; });
    Check(std::abs(traverseSum - foldSum) < 0.001f, "Traverse sum == Fold");

    // PreOrderFold
    float preSum = pq.PreOrderFold<float>(
        [](const float& val, const float& acc) { return acc + val; }, 0.0f);
    Check(std::abs(preSum - foldSum) < 0.001f, "PreOrderFold sum == Fold");

    // PostOrderFold
    float postSum = pq.PostOrderFold<float>(
        [](const float& val, const float& acc) { return acc + val; }, 0.0f);
    Check(std::abs(postSum - foldSum) < 0.001f, "PostOrderFold sum == Fold");

    // Pre/PostOrderTraverse
    float preTraverseSum = 0.0f, postTraverseSum = 0.0f;
    pq.PreOrderTraverse([&](const float& val) { preTraverseSum += val; });
    pq.PostOrderTraverse([&](const float& val) { postTraverseSum += val; });
    Check(std::abs(preTraverseSum - foldSum) < 0.001f, "PreOrderTraverse sum == Fold");
    Check(std::abs(postTraverseSum - foldSum) < 0.001f, "PostOrderTraverse sum == Fold");

    // Copy constructor
    lasd::PQHeap<float> copy(pq);
    Check(copy.Size() == pq.Size(), "Copy constructor: size match");

    // Move constructor
    lasd::PQHeap<float> moved(std::move(copy));
    Check(moved.Size() == pq.Size() && copy.Empty(), "Move constructor");

    // Copy assignment
    lasd::PQHeap<float> assigned;
    assigned = pq;
    Check(assigned.Size() == pq.Size(), "Copy assignment: size match");

    // Move assignment
    lasd::PQHeap<float> movedAssign;
    movedAssign = std::move(assigned);
    Check(movedAssign.Size() == pq.Size() && assigned.Empty(), "Move assignment");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() empties the PQ");

    // Exceptions
    bool thrown = false;
    try {
        movedAssign.Tip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "Tip() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.TipNRemove();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "TipNRemove() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.RemoveTip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "RemoveTip() on empty PQ throws exception");

    thrown = false;
    try {
        pq.Change(99, 0.0f);
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    Check(thrown, "Change(i out-of-range) throws exception");

    std::cout << std::endl;
}

void TestPQHeapDouble() {
    std::cout << "==== Test PQHeap<double> ====" << std::endl;

    lasd::PQHeap<double> pq;

    // Insert (copy)
    pq.Insert(3.75);
    pq.Insert(1.1);
    pq.Insert(4.9);
    pq.Insert(2.8);
    pq.Insert(0.6);

    Check(pq.Size() == 5, "Insert copy: Size == 5");
    Check(std::abs(pq.Tip() - 4.9) < 0.001, "Tip() == 4.9");
    Check(!pq.Empty(), "Empty() == false");

    // Insert (move)
    double x = 7.25;
    pq.Insert(std::move(x));
    Check(std::abs(pq.Tip() - 7.25) < 0.001, "Insert move: Tip() == 7.25");

    // TipNRemove
    double max = pq.TipNRemove();
    Check(std::abs(max - 7.25) < 0.001, "TipNRemove() == 7.25");
    Check(std::abs(pq.Tip() - 4.9) < 0.001, "Tip() == 4.9 after TipNRemove");

    // RemoveTip
    pq.RemoveTip();
    Check(std::abs(pq.Tip() - 3.75) < 0.001, "RemoveTip: Tip() == 3.75");

    // Change (copy)
    pq.Change(1, 10.0);
    Check(std::abs(pq.Tip() - 10.0) < 0.001, "Change(i, val): Tip() == 10.0");

    // Change (move)
    double y = 1.3;
    pq.Change(2, std::move(y));
    Check(pq.Exists(1.3), "Change(i, move) inserted 1.3");
    Check(std::abs(pq.Tip() - 10.0) < 0.001, "Change move: Tip remains 10.0");

    // Fold: sum
    double foldSum = pq.Fold<double>([](const double& val, const double& acc) {
        return acc + val;
    }, 0.0);
    Check(std::abs(foldSum - 16.15) < 0.001, "Fold<double> sum");

    // Traverse
    double traverseSum = 0.0;
    pq.Traverse([&](const double& val) { traverseSum += val; });
    Check(std::abs(traverseSum - foldSum) < 0.001, "Traverse sum == Fold");

    // PreOrderFold
    double preSum = pq.PreOrderFold<double>(
        [](const double& val, const double& acc) { return acc + val; }, 0.0);
    Check(std::abs(preSum - foldSum) < 0.001, "PreOrderFold sum == Fold");

    // PostOrderFold
    double postSum = pq.PostOrderFold<double>(
        [](const double& val, const double& acc) { return acc + val; }, 0.0);
    Check(std::abs(postSum - foldSum) < 0.001, "PostOrderFold sum == Fold");

    // Pre/PostOrderTraverse
    double preTraverseSum = 0.0, postTraverseSum = 0.0;
    pq.PreOrderTraverse([&](const double& val) { preTraverseSum += val; });
    pq.PostOrderTraverse([&](const double& val) { postTraverseSum += val; });
    Check(std::abs(preTraverseSum - foldSum) < 0.001, "PreOrderTraverse sum == Fold");
    Check(std::abs(postTraverseSum - foldSum) < 0.001, "PostOrderTraverse sum == Fold");

    // Copy constructor
    lasd::PQHeap<double> copy(pq);
    Check(copy.Size() == pq.Size(), "Copy constructor: size match");

    // Move constructor
    lasd::PQHeap<double> moved(std::move(copy));
    Check(moved.Size() == pq.Size() && copy.Empty(), "Move constructor");

    // Copy assignment
    lasd::PQHeap<double> assigned;
    assigned = pq;
    Check(assigned.Size() == pq.Size(), "Copy assignment: size match");

    // Move assignment
    lasd::PQHeap<double> movedAssign;
    movedAssign = std::move(assigned);
    Check(movedAssign.Size() == pq.Size() && assigned.Empty(), "Move assignment");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() empties the PQ");

    // Exceptions
    bool thrown = false;
    try {
        movedAssign.Tip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "Tip() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.TipNRemove();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "TipNRemove() on empty PQ throws exception");

    thrown = false;
    try {
        movedAssign.RemoveTip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "RemoveTip() on empty PQ throws exception");

    thrown = false;
    try {
        pq.Change(99, 0.0);
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    Check(thrown, "Change(i out-of-range) throws exception");

    std::cout << std::endl;
}

void TestPQHeapString() {
    std::cout << "==== Test PQHeap<string> ====" << std::endl;

    lasd::PQHeap<std::string> pq;

    // Insert (copy)
    pq.Insert("mela");
    pq.Insert("banana");
    pq.Insert("kiwi");
    pq.Insert("ananas");
    pq.Insert("uva");

    Check(pq.Size() == 5, "Insert copy: Size == 5");
    Check(pq.Tip() == "uva", "Tip() == 'uva'");
    Check(!pq.Empty(), "Empty() == false");

    // Insert (move)
    std::string word = "zebra";
    pq.Insert(std::move(word));
    Check(pq.Tip() == "zebra", "Insert move: Tip() == 'zebra'");

    // TipNRemove
    std::string top = pq.TipNRemove();
    Check(top == "zebra", "TipNRemove() == 'zebra'");
    Check(pq.Tip() == "uva", "Tip() after TipNRemove == 'uva'");

    // RemoveTip
    pq.RemoveTip();
    Check(pq.Tip() == "mela", "RemoveTip: Tip() == 'mela'");

    // Change (copy)
    pq.Change(1, "pesca");
    Check(pq.Tip() == "pesca", "Change(i, val): Tip() == 'pesca'");

    // Change (move)
    std::string str = "pera";
    pq.Change(0, std::move(str));
    Check(pq.Exists("pera"), "Change(i, move) inserts 'pera'");
    Check(pq.Tip() == "pera", "Tip remains 'pera'");

    // Fold: concatenation
    std::string concat = pq.Fold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, std::string());
    Check(!concat.empty(), "Fold<string> produces a concatenated string");

    // Traverse
    std::string joined;
    pq.Traverse([&](const std::string& val) {
        joined += val + "-";
    });
    Check(joined.find("mela") != std::string::npos, "Traverse contains 'mela'");

    // PreOrder / PostOrder Fold
    std::string pre = pq.PreOrderFold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, "");
    std::string post = pq.PostOrderFold<std::string>([](const std::string& val, const std::string& acc) {
        return acc + val;
    }, "");
    Check(!pre.empty() && !post.empty(), "Pre/PostOrderFold return concatenations");

    // Pre/PostOrderTraverse
    std::string preTrav, postTrav;
    pq.PreOrderTraverse([&](const std::string& val) { preTrav += val + "/"; });
    pq.PostOrderTraverse([&](const std::string& val) { postTrav += val + "/"; });
    Check(!preTrav.empty() && !postTrav.empty(), "Pre/PostOrderTraverse ok");

    // Copy constructor
    lasd::PQHeap<std::string> copy(pq);
    Check(copy.Size() == pq.Size(), "Copy constructor: Size match");

    // Move constructor
    lasd::PQHeap<std::string> moved(std::move(copy));
    Check(moved.Size() == pq.Size() && copy.Empty(), "Move constructor");

    // Copy assignment
    lasd::PQHeap<std::string> assigned;
    assigned = pq;
    Check(assigned.Size() == pq.Size(), "Copy assignment: Size match");

    // Move assignment
    lasd::PQHeap<std::string> movedAssign;
    movedAssign = std::move(assigned);
    Check(movedAssign.Size() == pq.Size() && assigned.Empty(), "Move assignment");

    // Clear
    movedAssign.Clear();
    Check(movedAssign.Empty(), "Clear() PQ<string>");

    // Exceptions
    bool thrown = false;
    try {
        movedAssign.Tip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "Tip() on empty PQ<string>");

    thrown = false;
    try {
        movedAssign.TipNRemove();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "TipNRemove() on empty PQ<string>");

    thrown = false;
    try {
        movedAssign.RemoveTip();
    } catch (const std::length_error&) {
        thrown = true;
    }
    Check(thrown, "RemoveTip() on empty PQ<string>");

    thrown = false;
    try {
        pq.Change(99, "x");
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    Check(thrown, "Change(i out-of-range) PQ<string>");

    std::cout << std::endl;
}

void RunAllTests2() {

    TestResetCounter();

    TestHeapVecInt();
    TestHeapVecFloat();
    TestHeapVecDouble();
    TestHeapVecString();
    TestHeapVecSortEdgeCases();

    TestPQHeapInt();
    TestPQHeapFloat();
    TestPQHeapDouble();
    TestPQHeapString();

    std::cout << "== TOTAL RESULTS ES2 ==" << std::endl;
    std::cout << "Total tests executed: " << totalTests << std::endl;
    std::cout << "Failed tests:         " << failedTests << std::endl;
    std::cout << std::endl;
}

void RunAllTests()
{
    TestResetCounter();

    RunAllTests1();

    int testpart1 = totalTests;
    int testerror1 = failedTests;

    TestResetCounter();
    RunAllTests2();

    std::cout << "== TOTAL RESULTS ==" << std::endl;
    std::cout << "Total tests executed: " << totalTests + testpart1 << std::endl;
    std::cout << "Failed tests:         " << failedTests + testerror1 << std::endl;
    std::cout << std::endl;

}

} // namespace lasd
