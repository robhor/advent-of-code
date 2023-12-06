cc_library(
    name = "day06-lib",
    hdrs = ["day06.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day06",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day06-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day06_test.cc"],
    deps = [
        ":day06-lib",
        "@googletest//:gtest_main",
    ],
)
