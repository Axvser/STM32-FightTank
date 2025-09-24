using VeloxDev.Core.MVVM;

namespace Controller.ViewModels;

public partial class MainViewModel
{
    [VeloxProperty] private TankViewModel _tank = new();
}