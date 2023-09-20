
//=============================================================================
//
//=============================================================================
enum ColorMatrixStyle
{
	AllPanel, OnlyCorners
};

//=============================================================================
// ColorizerPanel
//=============================================================================
class ColorizerPanel
{
public:
	bool	IsColorizerEnabled;
	bool	IsTransparencyEnabled;

	byte	Red;
	byte	Green;
	byte	Blue;
	byte	Alpha;

	ColorMatrixStyle PaintingStyle;

	Color	TextColor; // 글자 색

	ColorizerPanel()
	{
		IsColorizerEnabled = true;
		IsTransparencyEnabled = false;

		Red = 0;
		Green = 0;
		Blue = 0;
		Alpha = 255;

		PaintingStyle = OnlyCorners;

		TextColor = Color::White;
	}

	ColorizerPanel(byte red, byte green, byte blue, byte alpha, bool isColorizerEnabled, bool isTransparencyEnabled, ColorMatrixStyle paintingStyle)
	{
		// Sets RGBA
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;

		// Sets Options
		IsColorizerEnabled = isColorizerEnabled;
		IsTransparencyEnabled = isTransparencyEnabled;

		// Set Painting Style
		PaintingStyle = paintingStyle;
	}
};