cc_library(
    name = "day09-lib",
    hdrs = ["day09.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day09",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day09-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day09_test.cc"],
    deps = [
        ":day09-lib",
        "@googletest//:gtest_main",
    ],
)
