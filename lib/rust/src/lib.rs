#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn get_cycles_increasing() {
        let mut prev = unsafe { ubench_get_cycles() };

        for _ in 0..1000 {
            let cycles = unsafe { ubench_get_cycles() };
            assert!(cycles > prev);
            prev = cycles;
        }
    }
    #[test]
    fn stat_basic() {
        let values = [
            43256184, 95192806, 3270895, 91202119, 95206400, 68841354, 28840743, 90199941, 6545494,
            46519111, 9776299, 10151305, 16450599, 94646406, 63502117, 23370966, 35345217,
            14523679, 1646259, 25930884, 70536203, 54407826, 7857181, 98350342, 67009617, 13207455,
            69313736, 72259771, 50940439, 4448831, 5426305, 62463665, 8958145, 54311016, 21739453,
            96632118, 21122411, 66062385, 80031438, 72057867, 65691976, 82770604, 50332848,
            88267191, 79217920, 92681913, 49568829, 81103759, 1675719, 94089487, 26585608, 6328820,
            76430296, 28953316,
        ];
        let sum = 2685253268;
        let total = 54;

        let mut stat = ubench_stat_t::default();
        let stat_ref = &mut stat as *mut ubench_stat_t;
        unsafe {
            ubench_stat_init(stat_ref);
        }

        for i in 0..54 {
            unsafe { ubench_stat_add(stat_ref, values[i]) }
        }
        assert_eq!(stat.total, total);
        assert_eq!(stat.sum, sum);
    }
}
