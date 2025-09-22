using System.Threading.Tasks;

namespace Controller.Services.CommAdapter;

public class WLAN : ICommService
{
    public Task<bool> TryConnectAsync(params object[] context)
    {
        throw new System.NotImplementedException();
    }

    public Task<object> SendAsync(object data)
    {
        throw new System.NotImplementedException();
    }
}