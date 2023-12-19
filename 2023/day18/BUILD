cc_library(
    name = "day18-lib",
    hdrs = ["day18.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day18",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day18-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day18_test.cc"],
    deps = [
        ":day18-lib",
        "@googletest//:gtest_main",
    ],
)
