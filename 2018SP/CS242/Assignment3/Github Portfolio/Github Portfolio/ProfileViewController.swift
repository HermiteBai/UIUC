//
//  FirstViewController.swift
//  Github Portfolio
//
//  Created by Lanxiao Bai on 3/8/18.
//  Copyright © 2018 HermiteBai. All rights reserved.
//

import UIKit
import Alamofire
import AlamofireImage

class ProfileViewController: UIViewController {
    @IBOutlet weak var avatar : UIImageView!
    @IBOutlet weak var nameLabel : UILabel!
    @IBOutlet weak var usernameLabel : UILabel!
    @IBOutlet weak var followingNumButton : UIButton!
    @IBOutlet weak var followerNumButton : UIButton!
    @IBOutlet weak var public_repo : UIButton!
    @IBOutlet weak var bio : UITextView!
    @IBOutlet weak var website : UILabel!
    @IBOutlet weak var email : UILabel!
    @IBOutlet weak var createDateLabel : UILabel!
    var loaded : Bool = false
    var avatar_url : String?
    var login : String = "HermiteBai"
    
    fileprivate func setupProfile(_ JSON: (Any)) {
        let response = JSON as! NSDictionary
        self.nameLabel.text = response.object(forKey: "name") as? String
        self.usernameLabel.text = response.object(forKey: "login") as? String
        self.bio.text = response.object(forKey: "bio") as? String
        self.bio.isEditable = false
        self.website.text = response.object(forKey: "html_url") as? String
        self.email.text = response.object(forKey: "email") as? String
        self.createDateLabel.text = response.object(forKey: "updated_at") as? String
        self.followingNumButton.setTitle("\(response.object(forKey: "following")!)", for: .normal)
        self.followerNumButton.setTitle("\(response.object(forKey: "followers")!)", for: .normal)
        self.public_repo.setTitle("\(response.object(forKey: "public_repos")!)", for: .normal)
        self.avatar_url = response.object(forKey: "avatar_url") as? String
        Alamofire.request(self.avatar_url!, method: .get).responseImage { response in
            guard let image = response.result.value else {
                print("Meet error when loading avatar")
                return
            }
            self.avatar.image = image
        }
    }
    
    fileprivate func requestData(url : String) {
        Alamofire.request(url, method: .get).responseJSON {
            response in switch response.result {
            case .success(let JSON):
                self.setupProfile(JSON)
            case .failure(let error):
                print("Request failed with error: \(error)")
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        print("View did load")
        let url = "https://api.github.com/users/" + login
        requestData(url : url)
        avatar.layer.borderWidth = 1
        avatar.layer.masksToBounds = false
        avatar.layer.borderColor = UIColor.black.cgColor
        avatar.layer.cornerRadius = avatar.frame.height / 2
        avatar.clipsToBounds = true
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func gotoRepo(sender: AnyObject) {
        tabBarController?.selectedIndex = 1
    }
    
    @IBAction func restore() {
        //self.login = "HermiteBai"
        self.viewDidLoad()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        let url = "https://api.github.com/users/" + login
        requestData(url : url)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        let url = "https://api.github.com/users/" + login
        requestData(url : url)
    }
}

