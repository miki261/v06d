#include "main.h"
#include <cmath>
#include "rc.h"
#include <commdlg.h>
#include <numbers>

int number_dialog::idd() const {
	return IDD_NUMBER;
}

bool number_dialog::on_init_dialog() {
	set_int(IDC_EDIT1, number_of_circles);
	return true;
}

bool number_dialog::on_ok() {
	try {
		number_of_circles = get_int(IDC_EDIT1);
	}
	catch (...) {
		return false;
	}
	return true;
}

void main_window::on_paint(HDC hdc) {
	RECT rc;
	GetClientRect(*this, &rc);

	int cx = (rc.left + rc.right) / 2;
	int cy = (rc.top + rc.bottom) / 2;
	int big_radius = min(rc.right - rc.left, rc.bottom - rc.top) / 4;

	
	int old_mode = SetROP2(hdc, R2_NOTXORPEN);


	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HPEN old_pen = (HPEN)SelectObject(hdc, pen);
	HBRUSH brush = CreateSolidBrush(color);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);

	for (int i = 0; i < num_circles; ++i) {
		double angle = 2 * std::numbers::pi * i / num_circles;
		int x = static_cast<int>(cx + big_radius * cos(angle));
		int y = static_cast<int>(cy + big_radius * sin(angle));
		Ellipse(hdc, x - big_radius, y - big_radius, x + big_radius, y + big_radius);
	}


	SelectObject(hdc, old_pen);
	SelectObject(hdc, old_brush);
	DeleteObject(pen);
	DeleteObject(brush);
	SetROP2(hdc, old_mode);
}


void main_window::on_command(int id) {
	switch (id) {
	case ID_COLOR: {
		CHOOSECOLOR cc{};
		COLORREF cust_colors[16] = {};
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = *this;
		cc.lpCustColors = cust_colors;
		cc.rgbResult = color;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;
		if (ChooseColor(&cc))
			color = cc.rgbResult;
		InvalidateRect(*this, nullptr, true);
		break;
	}
	case ID_NUMBER: {
		number_dialog dlg;
		dlg.number_of_circles = num_circles;
		if (dlg.do_modal((HINSTANCE)GetWindowLongPtr(*this, GWLP_HINSTANCE), *this) == IDOK) {
			num_circles = dlg.number_of_circles;
			InvalidateRect(*this, nullptr, true);
		}
		break;
	}
	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void main_window::on_destroy() {
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int) {
	vsite::nwp::application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (UINT_PTR)LoadMenu(hi, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
