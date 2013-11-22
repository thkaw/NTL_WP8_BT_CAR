using NTL_WP8_BT_CAR.Resources;

namespace NTL_WP8_BT_CAR
{
    /// <summary>
    /// 提供字串資源的存取權。
    /// </summary>
    public class LocalizedStrings
    {
        private static AppResources _localizedResources = new AppResources();

        public AppResources LocalizedResources { get { return _localizedResources; } }
    }
}