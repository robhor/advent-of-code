cc_library(
    name = "day02-lib",
    hdrs = ["day02.cc"],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "day02",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day02-lib",
    ],
)

cc_test(
    name = "test",
    srcs = ["day02_test.cc"],
    deps = [
        ":day02-lib",
        "@com_google_googletest//:gtest_main",
    ],
)
