cc_library(
    name = "day07-lib",
    hdrs = ["day07.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day07",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day07-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day07_test.cc"],
    deps = [
        ":day07-lib",
        "@googletest//:gtest_main",
    ],
)
