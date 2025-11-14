# LowLevel.h function tips
@warning This is mainly for developers

1. `MwLLSetSizeHints`, `MwLLMakeBorderless`, `MwLLMakeToolWindow`, `MwLLMakePopup` have to be called between `MwLLBeginStateChange` and `MwLLEndStateChange`

