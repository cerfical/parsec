#pragma once

#include <array>
#include <string>
#include <string_view>

namespace parsec::src_gen {

    /**
     * @brief Contains a set of various settings to customize the output produced by a generator.
     */
    class ConfigStore {
    public:

        ConfigStore() = default;

        ConfigStore(const ConfigStore& other) = delete;
        ConfigStore& operator=(const ConfigStore& other) = delete;

        ConfigStore(ConfigStore&& other) noexcept = default;
        ConfigStore& operator=(ConfigStore&& other) noexcept = default;

        ~ConfigStore() = default;


        /**
         * @brief Name for a special token marking the end of file.
         */
        ConfigStore& eofTokenName(std::string_view value) {
            data_[0] = value;
            return *this;
        }

        std::string_view eofTokenName() const {
            return data_[0];
        }


        /**
         * @brief Name for a token that defines which character sequences are considered whitespace.
         */
        ConfigStore& wsTokenName(std::string_view value) {
            data_[1] = value;
            return *this;
        }

        std::string_view wsTokenName() const {
            return data_[1];
        }



    private:
        static constexpr std::size_t ConfigSize = 2;

        std::array<std::string, ConfigSize> data_ = { "Eof", "Ws" };
    };

}
