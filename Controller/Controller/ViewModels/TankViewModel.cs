using System.Threading;
using System.Threading.Tasks;
using VeloxDev.Core.Mono;
using VeloxDev.Core.MVVM;

namespace Controller.ViewModels;

[MonoBehaviour]
public partial class TankViewModel
{
    partial void Update()
    {
        if (!IsContextChanged) return;
        IsContextChanged = false;
        PostCommand.Execute(null);
    }

    [VeloxProperty] private double _leftTrack = 0d;
    [VeloxProperty] private double _rightTrack = 0d;
    [VeloxProperty] private double _turretH = 0d;
    [VeloxProperty] private double _turretV = 0d;
    [VeloxProperty] private double _firePower = 0d;
    [VeloxProperty] private bool _isContextChanged = false;

    partial void OnLeftTrackChanged(double oldValue, double newValue)
    {
        IsContextChanged = true;
    }

    partial void OnRightTrackChanged(double oldValue, double newValue)
    {
        IsContextChanged = true;
    }

    partial void OnTurretHChanged(double oldValue, double newValue)
    {
        IsContextChanged = true;
    }

    partial void OnTurretVChanged(double oldValue, double newValue)
    {
        IsContextChanged = true;
    }

    partial void OnFirePowerChanged(double oldValue, double newValue)
    {
        IsContextChanged = true;
    }

    [VeloxCommand]
    private async Task Post(object? parameter, CancellationToken ct)
    {
        
    }
}