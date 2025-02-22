﻿using Solar.Models;
using Solar.Repositories;
using Solar.Views;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Xamarin.Forms;

namespace Solar.ViewModels
{
    public class PanelsViewModel : BaseViewModel
    {

        public ObservableCollection<Panel> Panels { get; set; }
        public ICommand AddCommand { get; set; }
        public ICommand RefreshCommand { get; set; }
        public ICommand MeasureCommand { get; set; }
        public ICommand GetCommand { get; set; }
        private Panel panel;
        private readonly IRepository repository;

        public Panel PanelSelected
        {
            get =>panel; 
            set => SetProperty(ref panel, value);
        }
        private bool _opening=true;

        public bool Opening
        {
            get { return _opening; }
            set { SetProperty(ref _opening, value); }
        }

        public PanelsViewModel(IRepository repository)
        {
            this.repository = repository;
            Panels = new ObservableCollection<Panel>
            {
                new Panel { Place = "Wherever", Reference = "1192" }
            };
            AddCommand = new Command(async () => await AddPanel());
            MeasureCommand = new Command<Panel>(async (p) => await MeasureUI(p));
            RefreshCommand = new Command(async () => await RefreshList());
            GetCommand = new Command<Panel>(async (p) => await GetPanelData(p));
            RefreshCommand.Execute(null);
        }


        private bool isRefreshing;

        public bool IsRefreshing
        {
            get { return isRefreshing; }
            set { SetProperty(ref isRefreshing, value); }
        }
        private async Task GetPanelData(Panel p)
        {
            await Application.Current.MainPage.Navigation.PushModalAsync(new CollectData(p));
        }
        private async Task MeasureUI(Panel p)
        {
            await Application.Current.MainPage.Navigation.PushModalAsync(new GetInfo(p));
        }

        private async Task RefreshList()
        {
            IsRefreshing = true;
            var x = await repository.GetPanels();
            //var x = await repository.GetPanels();
            foreach (var item in x)
            {
                if (!Panels.Contains(item))
                {
                    Panels.Add(item);
                }
            }
            IsRefreshing = false;
        }

        private async Task AddPanel()
        {
            Opening = false;
            await Application.Current.MainPage.Navigation.PushModalAsync(new Measure());
            Opening = true;
        }
        
    }
}
