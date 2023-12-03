cc_library(
    name = "day03-lib",
    hdrs = ["day03.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day03",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day03-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day03_test.cc"],
    deps = [
        ":day03-lib",
        "@googletest//:gtest_main",
    ],
)
