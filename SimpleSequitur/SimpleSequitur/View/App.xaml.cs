using SimpleSequitur.ViewModel;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace SimpleSequitur
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            SimpleSequitur.MainWindow window = new MainWindow();
            StringPackerViewModel VM = new StringPackerViewModel();
            window.DataContext = VM;
            window.Show();
        }
    }
}
