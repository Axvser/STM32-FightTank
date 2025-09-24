using Avalonia.Controls;
using Controller.ViewModels;

namespace Controller.Views;

public partial class MainView : UserControl
{
    public MainView()
    {
        InitializeComponent();
        if (DataContext is MainViewModel { Tank: { } tv })
        {
            tv.CanMonoBehaviour = true;
        }
    }
}