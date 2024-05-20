test_source = files('test_ics.c')

test_runner_script = join_paths(meson.source_root(), 'subprojects', 'unity', 'auto', 'generate_test_runner.rb')

# Define a custom target to generate the test runner using Unity's script
test_runner_output = custom_target('generate-test-runner',
    output: 'AllTestsRunner.c',
    command: ['ruby', test_runner_script, test_source, '@OUTPUT@'],
    input: test_source,
)

# Create the test executable with necessary dependencies and static libraries
test_executable = executable('test-suite',
    sources: [test_runner_output, test_source],
    dependencies: [unity_dep, a_dep, b_dep],
)

test('test-suite', test_executable)