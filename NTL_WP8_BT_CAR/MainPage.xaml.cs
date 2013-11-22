///////////////////////////////
//
//This programe part of communication code was write by 
//http://developer.nokia.com/Community/Wiki/Windows_Phone_8_communicating_with_Arduino_using_Bluetooth
//
//Extend bluetooth car remote module By Nathaniel Chen.
//Author Email:s99115247@stu.edu.tw
//Author Blog:http://www.dotblogs.com.tw/thkaw/
//
//History－
//2013/11/22 0.5Beta=>First release.
//
//
//MSS Get 1st win!~~gogogo!!
//
///////////////////////////////


using System;
using System.Windows;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Windows.Networking.Proximity;
using System.Diagnostics;
using BluetoothConnectionManager;

namespace NTL_WP8_BT_CAR
{
    public partial class MainPage : PhoneApplicationPage
    {
        private ConnectionManager connectionManager;


        public MainPage()
        {
            InitializeComponent();
            //connectionManager為與Arduino溝通轉換成Serial形式訊息傳出去的物件
            connectionManager = new ConnectionManager();

            //新增接收訊息的事件
            connectionManager.MessageReceived += connectionManager_MessageReceived;
        }

        async void connectionManager_MessageReceived(string message)
        {
            //你可以在這邊抓從arduino來的訊息
            Debug.WriteLine("Message received:" + message);
            string[] messageArray = message.Split(':');
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            connectionManager.Initialize();
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            connectionManager.Terminate();
        }

        private void ConnectAppToDeviceButton_Click_1(object sender, RoutedEventArgs e)
        {
            //連線藍芽裝置
            AppToDevice();
        }

        bool isConnect = false;
        private async void AppToDevice()
        {
            //注意，此副程式不宜呼叫過快，否則會internal exception.
            try
            {
                if (!isConnect)
                {
                    connectionManager.Initialize();
                    ConnectAppToDeviceButton.Content = "Connecting...";
                    PeerFinder.AlternateIdentities["Bluetooth:Paired"] = "";
                    var pairedDevices = await PeerFinder.FindAllPeersAsync();

                    if (pairedDevices.Count == 0)
                    {
                        Debug.WriteLine("No paired devices were found.");
                    }
                    else
                    {
                        foreach (var pairedDevice in pairedDevices)
                        {
                            if (pairedDevice.DisplayName == DeviceName.Text)
                            {
                                connectionManager.Connect(pairedDevice.HostName);
                                ConnectAppToDeviceButton.Content = "Disconnect";
                                DeviceName.IsReadOnly = true;
                                isConnect = true;
                                // ConnectAppToDeviceButton.IsEnabled = false;
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    connectionManager.Terminate();
                    ConnectAppToDeviceButton.Content = "Connect";
                    DeviceName.IsReadOnly = false;
                    isConnect = false;
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Oops, Somethings AppToDevice() wrong.");
            }

        }

        private async void btn_forward_Click(object sender, RoutedEventArgs e)
        {
            await connectionManager.SendCommand("Forward");
        }

        private async void btn_back_Click(object sender, RoutedEventArgs e)
        {
            await connectionManager.SendCommand("Back");
        }

        private async void btn_left_Click(object sender, RoutedEventArgs e)
        {
            await connectionManager.SendCommand("Left");
        }

        private async void btn_right_Click(object sender, RoutedEventArgs e)
        {
            await connectionManager.SendCommand("Right");
        }

        private async void btn_stop_Click(object sender, RoutedEventArgs e)
        {
            await connectionManager.SendCommand("Stop");
        }


        private void btn_setPower_Click(object sender, RoutedEventArgs e)
        {
            changePower();
        }
        private async void changePower()
        { await connectionManager.SendCommand("PowerVal;" + (int)slider_1.Value); }


        private void slider_1_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            tb_pwval.Text = ((int)slider_1.Value).ToString();
        }



    }
}