#include <lvgl.h>
#include <lv_demos.h>
#include <unistd.h>

void on_benchmark_end(const lv_demo_benchmark_summary_t *summary)
{
	lv_demo_benchmark_summary_display(summary);
	exit(0);
}

int main(void)
{
	lv_init();
#if LV_USE_SDL
	lv_sdl_window_create(1920, 1080);
#elif LV_USE_WAYLAND
	lv_display_t *disp = lv_wayland_window_create(1920, 1080, "EJ LVGL Benchmark", NULL);
	lv_wayland_window_set_maximized(disp, false);
#endif

	lv_demo_benchmark_set_end_cb(on_benchmark_end);
	lv_demo_benchmark();

	while (1) {
#if LV_USE_WAYLAND
		const uint32_t time_to_next = lv_wayland_timer_handler();
#else
		const uint32_t time_to_next = lv_timer_handler();
#endif

		if (time_to_next == LV_NO_TIMER_READY) {
			usleep(LV_DEF_REFR_PERIOD * 1000);
		} else {
			usleep(time_to_next * 1000);
		}
	}
}
