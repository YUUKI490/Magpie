#pragma once

namespace Magpie {

class CursorManager {
public:
	CursorManager() = default;
	CursorManager(const CursorManager&) = delete;
	CursorManager(CursorManager&&) = delete;

	~CursorManager() noexcept;

	void Update() noexcept;

	void OnScalingPosChanged() noexcept;

	void OnSrcStartMove() noexcept;

	void OnSrcEndMove() noexcept;

	void OnStartMove() noexcept;

	void OnEndResizeMove() noexcept;

	void OnSrcRectChanged() noexcept;

	// 光标不在缩放窗口上或隐藏时为 NULL
	HCURSOR CursorHandle() const noexcept {
		return _hCursor;
	}

	// 屏幕坐标
	POINT CursorPos() const noexcept {
		return _cursorPos;
	}

	bool IsCursorCaptured() const noexcept {
		return _isUnderCapture;
	}

	bool IsCursorCapturedOnForeground() const noexcept {
		return _isCapturedOnForeground;
	}

	bool IsCursorOnOverlay() const noexcept {
		return _isOnOverlay;
	}
	void IsCursorOnOverlay(bool value) noexcept;

	bool IsCursorCapturedOnOverlay() const noexcept {
		return _isCapturedOnOverlay;
	}
	void IsCursorCapturedOnOverlay(bool value) noexcept;

	int16_t SrcHitTest() const noexcept {
		return _lastCompletedHitTestResult;
	}

	// 系统光标被替换为透明光标时，返回替换前保存的副本，用于绘制光标形状。
	// 不是系统光标时原样返回。
	static HCURSOR ResolveOriginalCursor(HCURSOR hCursor) noexcept;

private:
	void _ShowSystemCursor(bool show, bool onDestory = false);

	// 将系统光标替换为透明光标或还原。Virtual Desktop 等串流软件会自行绘制系统光标，
	// 仅靠 ShowSystemCursor 无法隐藏，导致出现两个光标。
	void _SetSystemCursorsTransparent(bool transparent, bool onDestory = false) noexcept;

	void _AdjustCursorSpeed() noexcept;

	void _RestoreCursorSpeed() noexcept;

	void _ReliableSetCursorPos(POINT pos) const noexcept;

	winrt::fire_and_forget _SrcHitTestAsync(POINT screenPos) noexcept;

	void _ClearHitTestResult() noexcept;

	void _UpdateCursorState() noexcept;

	void _ClipCursorForMonitors(POINT cursorPos) noexcept;

	void _ClipCursorOnSrcMoving() noexcept;

	void _UpdateCursorPos() noexcept;

	void _StartCapture(POINT& cursorPos) noexcept;

	bool _StopCapture(POINT& cursorPos, bool onDestroy = false) noexcept;

	void _SetClipCursor(const RECT& clipRect, bool is3DGameMode = false) noexcept;

	void _RestoreClipCursor() noexcept;

	HCURSOR _hCursor = NULL;
	POINT _cursorPos{ std::numeric_limits<LONG>::max() };

	// 用于确保拖拽源窗口和缩放窗口时光标位置稳定，使用相对于渲染矩形的局部坐标
	POINT _localCursorPosOnMoving{ std::numeric_limits<LONG>::max() };

	// 用于防止光标移动到边框的过程中闪烁
	std::chrono::steady_clock::time_point _sizeCursorStartTime{};

	RECT _lastClip{ std::numeric_limits<LONG>::max() };
	RECT _lastRealClip{ std::numeric_limits<LONG>::max() };

	int _originCursorSpeed = 0;

	uint32_t _nextHitTestId = 0;
	uint32_t _lastCompletedHitTestId = 0;
	POINT _lastCompletedHitTestPos{ std::numeric_limits<LONG>::max() };
	int16_t _lastCompletedHitTestResult = HTNOWHERE;

	bool _isUnderCapture = false;
	// 当缩放后的光标位置在交换链窗口上且没有被其他窗口挡住时应绘制光标
	bool _shouldDrawCursor = false;
	
	bool _isCapturedOnForeground = false;

	bool _isOnOverlay = false;
	bool _isCapturedOnOverlay = false;

	bool _isSystemCursorShown = true;

	bool _areSystemCursorsTransparent = false;

	static inline const HCURSOR _hDiagonalSize1Cursor = LoadCursor(NULL, IDC_SIZENWSE);
	static inline const HCURSOR _hDiagonalSize2Cursor = LoadCursor(NULL, IDC_SIZENESW);
	static inline const HCURSOR _hHorizontalSizeCursor = LoadCursor(NULL, IDC_SIZEWE);
	static inline const HCURSOR _hVerticalSizeCursor = LoadCursor(NULL, IDC_SIZENS);
};

}
