cc_library(
    name = "day08-lib",
    hdrs = ["day08.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day08",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day08-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day08_test.cc"],
    deps = [
        ":day08-lib",
        "@googletest//:gtest_main",
    ],
)
