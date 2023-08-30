#pragma once

#include <algorithm>
#include <cfloat>
#include <random>

namespace Piccolo {
    // 根据NumericType的类型, 定义 uniform_distribution 对应不同的分布
    template<typename NumericType>
    using uniform_distribution = typename std::conditional<std::is_integral<NumericType>::value,
                                                           std::uniform_int_distribution<NumericType>,
                                                           std::uniform_real_distribution<NumericType>>::type;

    /* 随机数生成器 */
    template<typename RandomEngine = std::default_random_engine>
    class RandomNumberGenerator {
    private:
        RandomEngine m_engine;

    public:
        // 显示构造函数
        template<typename... Params>
        explicit RandomNumberGenerator(Params&&... params) :
            m_engine(std::forward<Params>(params)...) {}

        // 随机数种子
        template<typename... Params>
        void seed(Params&&... seeding) {
            m_engine.seed(std::forward<Params>(seeding)...);
        }

        // 生成一个随机数, 满足自定义分布 DistributionFunc
        template<typename DistributionFunc, typename... Params>
        typename DistributionFunc::result_type distribution(Params&&... params) {
            DistributionFunc dist(std::forward<Params>(params)...);
            return dist(m_engine);
        }

        // 生成一个随机数, 满足平均分布
        template<typename NumericType>
        NumericType uniformDistribution(NumericType lower, NumericType upper) {
            if (lower == upper) return lower;
            return distribution<uniform_distribution<NumericType>>(lower, upper);
        }

        // 生成一个[0, 1]之间的随机数, 满足平均分布
        float uniformUnit() {
            // 使用nextafter函数, 生成一个比1大一点点的浮点数, 避免精度误差
            return uniformDistribution(0.f, std::nextafter(1.f, FLT_MAX));
        }

        // 生成一个[-1, 1]之间的随机数, 满足平均分布
        float uniformSymmetry() {
            return uniformDistribution(-1.f, std::nextafter(1.f, FLT_MAX));
        }

        // 生成一个bool随机数, 满足伯努利分布(01分布), 概率为 probability
        bool bernoullDistribution(float probability) {
            return distribution<std::bernoulli_distribution>(probability);
        }

        // 生成一个随机数, 满足正态分布, 均值为 mean, 标准差为 stddev
        float normalDistribution(float mean, float stddev) {
            return distribution<std::normal_distribution<float>>(mean, stddev);
        }

        // 为range内的每一个元素生成一个随机数, 满足自定义分布 DistributionFunc
        template<typename DistributionFunc, typename Range, typename... Params>
        void generator(Range&& range, Params&&... params) {
            DistributionFunc dist(std::forward<Params>(params)...);
            return std::generate(std::begin(range), std::end(range), [&] { return dist(m_engine); });
        }
    };

    /* 随机数生成器, 满足自定义分布 DistributionFunc */
    template<typename DistributionFunc, typename RandomEngine = std::default_random_engine, typename SeedType = std::seed_seq>
    class DistRandomNumberGenerator {
        using ResultType = typename DistributionFunc::result_type;

    private:
        RandomEngine      m_engine;
        DistributionFunc* m_dist = nullptr;

    public:
        // 显示构造函数
        template<typename... Params>
        explicit DistRandomNumberGenerator(SeedType&& seeding, Params&&...) :
            m_engine(seeding) {}

        ~DistRandomNumberGenerator() { CHAOS_DELETE_T(m_dist); }

        // 随机数种子
        template<typename... Params>
        void seed(Params&&... params) {
            m_engine.seed(std::forward<Params>(params)...);
        }

        // 生成一个随机数, 满足自定义分布 DistributionFunc
        ResultType next() { return (*m_dist)(m_engine); }
    };

    // 缺省随机数生成器, 使用std::mt19937引擎
    using DefaultRNG = RandomNumberGenerator<std::mt19937>;
} // namespace Piccolo