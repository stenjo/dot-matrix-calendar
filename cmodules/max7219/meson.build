project('Unity example', 'c',
  license: 'MIT',
  default_options: [
    'c_std=c99',
    'warning_level=3',
  ],
  meson_version: '>= 0.49.0'
)

unity_subproject = subproject('unity')
unity_dependency = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')

src1 = files([
  'src' / 'matrix.c',
  'test' / 'TestMatrix.c',
  'mocks' / 'Mockmax7219.c',
  'mocks' / 'Mockesp_timer.c',
  'cmock' / 'src' / 'cmock.c'
])

# src2 = files([
#   'src' / 'max7219.c',
#   'test' / 'TestMax7219.c',
# ])

inc = include_directories(
  'src', 
  'mocks', 
  'cmock/src', 
  '/Users/sten.johnsen/esp/esp-idf/components/driver/include/driver', 
  '/Users/sten.johnsen/esp/esp-idf/components/esp_timer/include',
  '/Users/sten.johnsen/esp/esp-idf/components/esp_common/include', 
  '/Users/sten.johnsen/git/esp32/build/config',
  '/Users/sten.johnsen/esp/esp-idf/tools/mocks/freertos/include', 
  '/Users/sten.johnsen/esp/esp-idf/components/freertos/FreeRTOS-Kernel/include')

test1 = executable('test1',
  sources: [
    src1,
    unity_gen_runner.process('test' / 'TestMatrix.c')
  ],
  include_directories: [ inc ],
  dependencies: [ unity_dependency ],
)

test('test1', test1)

# test2 = executable('test2',
#   sources: [
#     src2,
#     unity_gen_runner.process('test' / 'TestMax7219.c')
#   ],
#   include_directories: [ inc ],
#   dependencies: [ unity_dependency ],
# )

# test('test2', test2)

