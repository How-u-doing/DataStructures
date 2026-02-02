use std::slice;

#[no_mangle]
pub unsafe extern "C" fn select_nth_unstable_rust(
    ptr: *mut f64,
    len: usize,
    index: usize,
) {
    let slice = slice::from_raw_parts_mut(ptr, len);

    slice.select_nth_unstable_by(index, |a, b| a.partial_cmp(b).unwrap());
}
