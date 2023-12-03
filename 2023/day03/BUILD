cc_library(
    name = "day03-lib",
    hdrs = ["day03.cc"],
    deps = [
        "@com_google_absl//absl/strings",
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
        "@com_google_googletest//:gtest_main",
    ],
)
