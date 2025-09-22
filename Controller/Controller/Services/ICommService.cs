using System.Threading.Tasks;

namespace Controller.Services;

public interface ICommService
{
    Task<object> TryConnectAsync(params object[] context);
    public Task<object> SendAsync(params object[] context);
    public Task<object> ReceiveAsync(params object[] context);
}