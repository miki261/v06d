#pragma once
#include "nwpwin.h"
#include "nwpdlg.h"

class main_window : public vsite::nwp::window {
	COLORREF color = RGB(0, 0, 0);
	int num_circles = 12;

protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};

class number_dialog : public vsite::nwp::dialog {
public:
	int number_of_circles;
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};
