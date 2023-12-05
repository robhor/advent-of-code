cc_library(
    name = "day05-lib",
    hdrs = ["day05.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day05",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day05-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day05_test.cc"],
    deps = [
        ":day05-lib",
        "@googletest//:gtest_main",
    ],
)
