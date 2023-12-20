cc_library(
    name = "day20-lib",
    hdrs = ["day20.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day20",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day20-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day20_test.cc"],
    deps = [
        ":day20-lib",
        "@googletest//:gtest_main",
    ],
)
