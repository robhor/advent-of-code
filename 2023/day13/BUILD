cc_library(
    name = "day13-lib",
    hdrs = ["day13.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day13",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day13-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day13_test.cc"],
    deps = [
        ":day13-lib",
        "@googletest//:gtest_main",
    ],
)
