
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2017 Gleb Mishchenko, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// appleseed.foundation headers.
#include "foundation/utility/countof.h"
#include "foundation/utility/test.h"
#include "foundation/utility/zip.h"

// Boost headers.
#include "boost/filesystem.hpp"

// Standard headers.
#include <set>
#include <string>
#include <vector>

using namespace foundation;
using namespace std;
namespace bf = boost::filesystem;

TEST_SUITE(Foundation_Utility_Zip)
{
    const string ValidProjectFilePath = "unit tests/inputs/test_zip_valid_packed_project.appleseedz";

    TEST_CASE(Unzip)
    {
        const string TargetDirectory = "unit tests/outputs/test_zip_valid_packed_project.unpacked/";

        try
        {
            ASSERT_FALSE(bf::exists(TargetDirectory));

            unzip(ValidProjectFilePath, TargetDirectory);

            EXPECT_TRUE(bf::exists(TargetDirectory));
            EXPECT_FALSE(bf::is_empty(TargetDirectory));

            const string ExpectedFiles[] =
            {
                "01 - lambertiannrdf - arealight.appleseed",
                "geometry/sphere.obj",
                "geometry/Box002.binarymesh",
                "geometry/GeoSphere001.binarymesh",
                "geometry/dirpole reference sphere.obj",
                "geometry/Box001.binarymesh",
                "geometry/plane.obj",
                "geometry/Sphere002.binarymesh",
                "geometry/Plane002.binarymesh",
                "geometry/cube.obj",
                "geometry/Plane001.binarymesh"
            };

            const set<string> actual_files = recursive_ls(TargetDirectory);

            for (size_t i = 0; i < countof(ExpectedFiles); ++i)
                EXPECT_EQ(1, actual_files.count(ExpectedFiles[i]));

            bf::remove_all(TargetDirectory);
        }
        catch (const exception& e)
        {
            bf::remove_all(TargetDirectory);
            throw e;
        }
    }

    TEST_CASE(IsZipFile_GivenValidZipFile_ReturnsTrue)
    {
        EXPECT_TRUE(is_zip_file(ValidProjectFilePath.c_str()));
    }

    TEST_CASE(IsZipFile_GivenInvalidZipFile_ReturnsFalse)
    {
        EXPECT_FALSE(is_zip_file("unit tests/inputs/test_zip_not_zip_file.txt"));
    }

    TEST_CASE(GetFilenamesWithExtensionFromZip_OneFile)
    {
        const vector<string> appleseed_files =
            get_filenames_with_extension_from_zip(ValidProjectFilePath, ".appleseed");

        ASSERT_EQ(1, appleseed_files.size());
        EXPECT_EQ("01 - lambertiannrdf - arealight.appleseed", appleseed_files[0]);
    }

    TEST_CASE(GetFilenamesWithExtensionFromZip_SeveralFiles)
    {
        const string InvalidProjectFilePath = "unit tests/inputs/test_zip_invalid_packed_project.appleseedz";

        const vector<string> appleseed_files =
            get_filenames_with_extension_from_zip(InvalidProjectFilePath, ".appleseed");

        EXPECT_EQ(4, appleseed_files.size());
    }
}
