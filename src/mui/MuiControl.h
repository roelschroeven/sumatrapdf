/* Copyright 2021 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

class EventMgr;

class Control : public ILayout {
  public:
    // allows a control to opt-out from being notified about
    // input events, stored in wantedInputBits
    enum WantedInputBits {
        WantsMouseOverBit = 0,
        WantsMouseDownBit = 1,
        WantsMouseUpBit = 2,
        WantsMouseClickBit = 3,
        WantsMouseMoveBit = 4,
        WantedInputBitLast
    };

    // describes current state of a window, stored in stateBits
    enum ControlStateBits {
        MouseOverBit = 0,
        IsPressedBit = 1,
        // using IsHidden and not IsVisible so that 0 is default, visible state
        IsHiddenBit = 2,
        StateBitLast
    };

    explicit Control(Control* newParent = nullptr);
    ~Control() override;

    void SetParent(Control* newParent);
    void AddChild(Control* c, int pos = -1);
    void AddChild(Control* c1, Control* c2, Control* c3 = nullptr);
    [[nodiscard]] Control* GetChild(size_t idx) const;
    [[nodiscard]] size_t GetChildCount() const;

    void SetPosition(Rect p);

    virtual void Paint(Graphics* gfx, int offX, int offY);

    // ILayout
    Size Measure(Size availableSize) override;
    void Arrange(Rect finalRect) override;
    Size DesiredSize() override;

    // mouse enter/leave are used e.g. by a button to change the look when mouse
    // is over them. The intention is that in response to those a window should
    // only do minimal processing that affects the window itself, not the rest
    // of the system
    virtual void NotifyMouseEnter();
    virtual void NotifyMouseLeave();

    virtual void NotifyMouseMove(__unused int x, __unused int y) {
    }

    [[nodiscard]] bool WantsMouseClick() const;
    [[nodiscard]] bool WantsMouseMove() const;
    [[nodiscard]] bool IsMouseOver() const;
    void SetIsMouseOver(bool isOver);

    [[nodiscard]] bool IsVisible() const;
    void Hide();
    void Show();

    void SetToolTip(const WCHAR*);
    void SetNamedEventClick(const char*);

    void MeasureChildren(Size availableSize) const;
    void MapMyToRootPos(int& x, int& y) const;
    void MapRootToMyPos(int& x, int& y) const;

    u16 wantedInputBits; // WndWantedInputBits
    u16 stateBits;       // WndStateBits
    // windows with bigger z-order are painted on top, 0 is default
    i16 zOrder;

    ILayout* layout;
    Control* parent;

    WCHAR* toolTip;

    const char* namedEventClick;

    // we cache properties for the current style during SetStyle() which
    // makes if fast to access them anywhere without repeating the work
    // of searching the style inheritance chain
    CachedStyle* cachedStyle;
    bool SetStyle(Style* style);

    // only used by HwndWrapper but we need it here
    // TODO: figure out to not have it in every Control
    HWND hwndParent;

    // cursor to show when mouse is over this window.
    // only works if the window sets WantsMouseOverBit.
    // Control doesn't own hCursor in order to enable easy
    // sharing of cursor among many windows.
    HCURSOR hCursor;

    // position and size (relative to parent, might be outside of parent's bounds)
    Rect pos;

  protected:
    Vec<Control*> children;

    // desired size calculated in Measure()
    Size desiredSize;
};
