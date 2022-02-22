README
======
Below we give instructions for compiling each test:

### Spinbox inc/dec test
```
$: gcc -Wall test_inc_dec.c spinbox_mock.c time_spinbox.c
```

### Set initial end time test
```
$: gcc -Wall test_set_initial_end_time.c
```

### Edit end time test
```
$: gcc -Wall test_edit_end_time.c gen3_sdu_defs.c spinbox_mock.c time_spinbox.c
```
