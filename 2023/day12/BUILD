cc_library(
    name = "day12-lib",
    hdrs = ["day12.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day12",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day12-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day12_test.cc"],
    deps = [
        ":day12-lib",
        "@googletest//:gtest_main",
    ],
)
