using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;
using ManagedMessenger;

namespace MyMessengerWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public ManagedMessenger.ManagedMessenger msg;

        public ObservableCollection<User> UserCollection = new ObservableCollection<User>();

        public Dictionary<string, List<string>> MessengerHistory =
            new Dictionary<string, List<string>>();

        public ObservableCollection<string> displayedHistory = new ObservableCollection<string>();
        public ObservableCollection<string> DisplayedHistory
        {
            get
            {return displayedHistory;}
            set
            {displayedHistory = value;}
        }

        public MainWindow()
        {
            DataContext = this;
            InitializeComponent();
            TextPanel.Visibility = Visibility.Hidden;
            //Item Sources
            ActiveUsers.ItemsSource = UserCollection;
            MessengerHistoryList.ItemsSource = DisplayedHistory;
            //Initialise messenger
            msg = new ManagedMessenger.ManagedMessenger("127.0.0.1", 5222);
            msg.Connect();
            //Subscribe on events
            msg.OnLogin += Msg_OnLogin;
            msg.OnUsersRequested += Msg_OnUsersRequested;
            msg.OnReceivedMessage += Msg_OnReceivedMessage;
        }

        private void Msg_OnReceivedMessage(object sender, DataTwoEventArgs<string, ManagedMessage> e)
        {
            //if (MessengerHistory.ContainsKey(e.Data1))
            //{
            //
            //}
            //
            //this.Dispatcher.BeginInvoke(
            //  new Action(() =>
            //  {
            //      DisplayedHistory = new ObservableCollection<string>(MessengerHistory[((User)ActiveUsers.SelectedItem).UserId]);
            //  }));
        }

        private void Msg_OnUsersRequested(object sender, DataTwoEventArgs<ResultStatus, List<User>> e)
        {
            if (e.Data1 == ResultStatus.Ok)
                this.Dispatcher.BeginInvoke(new Action(() => e.Data2.ForEach(UserCollection.Add)));
        }

        private void Msg_OnLogin(object sender, DataEventArgs<ResultStatus> e)
        {
            if (e.Data == ResultStatus.Ok)
            {
                msg.BeginRequestUsers();
                //close flyout
                this.Dispatcher.BeginInvoke(
                new Action(() =>
                {
                    LoginFlyout.IsOpen = false;
                    LogOutButton.Visibility = Visibility.Visible;
                }));
            }
        }

        private void LoginFlyout_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string login = Login.Text;
                string password = Password.Password;
                //login function from dll;
                msg.BeginLogin(new User(login, password, new SecurityPolicy()));
            }
        }


        private void LogOutButton_Click(object sender, RoutedEventArgs e)
        {
            msg.Disconnect();
            //////////////////////////
            Login.Text = "";
            Password.Clear();
            LogOutButton.Visibility = Visibility.Hidden;
            LoginFlyout.IsOpen = true;
        }

        private void SendMessengeButton_Click(object sender, RoutedEventArgs e)
        {
            if (MessengeBox.Text.Length != 0)
            {
                msg.BeginSend((User)ActiveUsers.SelectedItem, MessengeBox.Text);
                //add messege to history
                    if (MessengerHistory.ContainsKey(((User)ActiveUsers.SelectedItem).UserId))
                        MessengerHistory[((User)ActiveUsers.SelectedItem).UserId].Add(MessengeBox.Text);
                    else
                    {
                        List<string> history = new List<string>();
                        history.Add(MessengeBox.Text);
                        MessengerHistory.Add(((User)ActiveUsers.SelectedItem).UserId, history);
                    }
                MessengeBox.Text = "";
                //reload current history
                this.Dispatcher.BeginInvoke(
                new Action(() =>
                {
                    DisplayedHistory = new ObservableCollection<string>(MessengerHistory[((User)ActiveUsers.SelectedItem).UserId]);
                }));
            }
        }

        private void MessengeBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                SendMessengeButton_Click(sender, e);
        }

        private void ActiveUsers_SelectionChanged(object sender, SelectionChangedEventArgs e)
        { 
            TextPanel.Visibility = Visibility.Visible;
            this.Dispatcher.BeginInvoke(
               new Action(() =>
               {
                   if(MessengerHistory.ContainsKey(((User)ActiveUsers.SelectedItem).UserId))
                       DisplayedHistory = new ObservableCollection<string>(MessengerHistory[((User)ActiveUsers.SelectedItem).UserId]);
               }));
        }


    }
}
