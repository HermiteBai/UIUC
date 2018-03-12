//
//  SecondViewController.swift
//  Github Portfolio
//
//  Created by Lanxiao Bai on 3/8/18.
//  Copyright © 2018 HermiteBai. All rights reserved.
//
import Foundation
import UIKit
import Alamofire
import WebKit

class RepositoryViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, WKNavigationDelegate {
    
    @IBOutlet var tableView : UITableView?
    @IBOutlet var navItem: UINavigationItem?
    var repoNamesArr = NSMutableArray()
    var repoDescribeArr = NSMutableArray()
    var repoOwnersArr = NSMutableArray()
    var repoURLsArr = NSMutableArray()
    var webview : WKWebView?
    
    fileprivate func gatherData(_ currRepo: NSDictionary) {
        self.repoNamesArr.add("\(currRepo.object(forKey: "name")!)")
        self.repoDescribeArr.add("\(currRepo.object(forKey: "description")!)")
        let currOwner = currRepo.object(forKey: "owner") as! NSDictionary
        self.repoOwnersArr.add("\(currOwner.object(forKey: "login")!)")
        self.repoURLsArr.add("\(currRepo.object(forKey: "svn_url")!)")
    }
    
    fileprivate func requestData(_ url: String) {
        Alamofire.request(url, method: .get).responseJSON {
            response in switch response.result {
            case .success(let JSON):
                let repos = JSON as! NSArray
                for repo in repos {
                    let currRepo = repo as! NSDictionary
                    if currRepo.object(forKey: "private")! as! Bool == false {
                        self.gatherData(currRepo)
                    }
                }
                self.tableView!.reloadData()
            case .failure(let error):
                print("Request failed with error: \(error)")
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView?.dataSource = self
        tableView?.delegate = self
        let url = "https://api.github.com/users/HermiteBai/repos"
        requestData(url)
        tableView?.allowsSelection = true
        self.view.addSubview(tableView!)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    private func numberOfSections(_ tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        //var cell = tableView.dequeueReusableCell(withIdentifier: "mycell")
        var cell : UITableViewCell! = tableView.dequeueReusableCell(withIdentifier: "mycell")
        if cell == nil {
            cell = UITableViewCell(style: .subtitle, reuseIdentifier: "mycell")
        }
        cell.textLabel?.text="\(repoNamesArr[indexPath.row])"
        cell.detailTextLabel?.text="Owner: \(repoOwnersArr[indexPath.row])\nDescription: \(repoDescribeArr[indexPath.row])"
        cell.detailTextLabel?.numberOfLines = 2
        cell.isUserInteractionEnabled = true
        return cell
    }
    
    private func tableView(_ tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
        return 300;
    }
    
    internal func tableView(_ tableView:UITableView, numberOfRowsInSection section:Int) -> Int
    {
        return repoNamesArr.count
    }
    private func tableView(_ tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        print("Triggered \(indexPath.row)")
    }
    
    internal func tableView(_ tableView: UITableView, willSelectRowAt indexPath: IndexPath) -> IndexPath? {
        //UIApplication.shared.open(URL(string : "\(repoURLsArr[indexPath.row])")!)
        let myURL = URL(string: "\(repoURLsArr[indexPath.row])")
        let webConfiguration = WKWebViewConfiguration()
        webview = WKWebView(frame: CGRect(origin: .zero, size: CGSize(width: self.view.frame.width, height: self.view.frame.height)), configuration: webConfiguration)
        let myRequest = URLRequest(url: myURL!)
        webview?.navigationDelegate = self
        webview?.load(myRequest)
        let goBackGesture = UISwipeGestureRecognizer(target: self, action: #selector(self.swipeGesture))
        goBackGesture.direction = UISwipeGestureRecognizerDirection.right
        self.view.addGestureRecognizer(goBackGesture)
        self.view.addSubview(webview!)
        return indexPath
    }
    
    @objc func swipeGesture(swip:UISwipeGestureRecognizer) {
        self.webview?.removeFromSuperview()
    }
    
    func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
        self.navItem?.title = self.webview?.title
        setNavBar()
    }
    
    @objc private func toBack() {
        if (self.webview?.canGoBack)! {
            self.webview?.goBack()
        }
    }
    
    @objc private func close() {
        self.webview?.removeFromSuperview()
        self.navItem?.leftBarButtonItem = nil
        self.navItem?.rightBarButtonItem = nil
        self.navItem?.title = "Repo"
    }
    
    func setNavBar() {
        let btnBack = UIBarButtonItem.init(title: "Back", style: .plain, target: self, action: #selector(toBack))
        let btnClose = UIBarButtonItem.init(title: "Close", style: .plain, target: self, action: #selector(close))
        self.navItem?.leftBarButtonItem = btnBack
        self.navItem?.rightBarButtonItem = btnClose
    }
}

