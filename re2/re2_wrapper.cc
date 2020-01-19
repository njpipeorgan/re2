// Compiler from Wolfram Language to C++
// 
// Copyright 2019 Tianhuan Lu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <cstring>

#include "re2/re2_wrapper.h"

extern "C" void* _re2_RE2_construct_impl(const char* pattern)
{
    try
    {
        return static_cast<void*>(new re2::RE2(
            pattern, re2::RE2::CannedOptions::Quiet));
    }
    catch(...)
    {
        return nullptr;
    }
}

extern "C" void _re2_RE2_destruct_impl(void* ptr)
{
    delete static_cast<re2::RE2*>(ptr);
}

extern "C" bool _re2_RE2_match_impl(const void* regex_ptr,
    const void* piece_ptr, size_t startpos, size_t endpos, int re_anchor,
    void* submatch, int nsubmatch)
{
    const auto& regex = *static_cast<const re2::RE2*>(regex_ptr);
    return regex.Match(*static_cast<const re2::StringPiece*>(piece_ptr),
        startpos, endpos, re2::RE2::Anchor(re_anchor),
        static_cast<re2::StringPiece*>(submatch), nsubmatch);
}

extern "C" bool _re2_RE2_ok(const void* regex_ptr)
{
    const auto& regex = *static_cast<const re2::RE2*>(regex_ptr);
    return regex.ok();
}

extern "C" void _re2_RE2_error(const void* regex_ptr, char* buffer,
    const size_t buffer_size)
{
    const auto& regex = *static_cast<const re2::RE2*>(regex_ptr);
    const auto& error_str = regex.error();
    if (buffer_size > error_str.size())
    {
        std::strcpy(buffer, error_str.c_str());
    }
    else if (buffer_size > 0u)
    {
        std::strncpy(buffer, error_str.c_str(), buffer_size - 1u);
        buffer[buffer_size - 1u] = '\0';
    }
}
